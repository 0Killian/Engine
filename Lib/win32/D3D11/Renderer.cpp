//
// Created by Killian on 02/12/2023.
//
#include "D3D11/Renderer.h"

#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <d3d11sdklayers.h>

#include "D3D11/Utils.h"

namespace NGN
{
    struct Vertex
    {
        DirectX::XMFLOAT3 Position;
        DirectX::XMFLOAT3 Normal;
        DirectX::XMFLOAT2 UV;
    };

    D3D11::Renderer::Renderer(
        void* hwnd,
        Logger& logger,
        std::shared_ptr<EventManager>
        eventManager,
        const Configuration& config,
        const size_t width,
        const size_t height,
        const size_t id)
        : EventListener({
            EventType::WINDOW_RESIZE
        }, eventManager)
        , m_Logger(logger)
        , m_EventManager(std::move(eventManager))
        , m_WindowID(id)
        , m_VSync(config.VSync)
    {
        THROW_IF_FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&m_Factory)));

        constexpr auto featureLevel = D3D_FEATURE_LEVEL_11_1;
        UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef NGN_DEBUG
        flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        THROW_IF_FAILED(D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            flags,
            &featureLevel,
            1,
            D3D11_SDK_VERSION,
            &m_Device,
            nullptr,
            &m_Context));

#ifdef NGN_DEBUG
        THROW_IF_FAILED(m_Device.As(&m_Debug));
#endif

        m_Width = width;
        m_Height = height;

        const DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {
            .Width = static_cast<uint32_t>(width),
            .Height = static_cast<uint32_t>(height),
            .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
            .Stereo = false,
            .SampleDesc = {
                .Count = 1,
                .Quality = 0
            },
            .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
            .BufferCount = 2,
            .Scaling = DXGI_SCALING_STRETCH,
            .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
            .AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
            .Flags = {}
        };

        m_FramesInFlight = swapChainDesc.BufferCount - 1; // -1 because we need to wait for the first frame to finish before we can start the next one

        constexpr DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapChainFullscreenDesc = {
            .RefreshRate = {
                .Numerator = 144,
                .Denominator = 1
            },
            .ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
            .Scaling = DXGI_MODE_SCALING_STRETCHED,
            .Windowed = true
        };

        THROW_IF_FAILED(m_Factory->CreateSwapChainForHwnd(
            m_Device.Get(),
            static_cast<HWND>(hwnd),
            &swapChainDesc,
            &swapChainFullscreenDesc,
            nullptr,
            &m_SwapChain));

        CreateSwapchainResources();

        String shaderName = "Main";
        m_ShaderProgram = std::make_unique<ShaderProgram>("Main", logger, *this, config);

        constexpr D3D11_INPUT_ELEMENT_DESC vertexInputLayoutInfo[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, Normal), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex, UV), D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        THROW_IF_FAILED(m_Device->CreateInputLayout(
            vertexInputLayoutInfo,
            _countof(vertexInputLayoutInfo),
            m_ShaderProgram->GetVertexShaderBlob()->GetBufferPointer(),
            m_ShaderProgram->GetVertexShaderBlob()->GetBufferSize(),
            &m_InputLayout));

        constexpr Vertex vertices[] = {
            { DirectX::XMFLOAT3{  0.0f,  0.5f, 0.0f }, DirectX::XMFLOAT3{ 0.25f, 0.39f, 0.19f }, DirectX::XMFLOAT2{ 0.5f, 0.0f } },
            { DirectX::XMFLOAT3{  0.5f, -0.5f, 0.0f }, DirectX::XMFLOAT3{ 0.44f, 0.75f, 0.35f }, DirectX::XMFLOAT2{ 1.0f, 1.0f } },
            { DirectX::XMFLOAT3{ -0.5f, -0.5f, 0.0f }, DirectX::XMFLOAT3{ 0.38f, 0.55f, 0.20f }, DirectX::XMFLOAT2{ 0.0f, 1.0f } }
        };

        D3D11_BUFFER_DESC bufferInfo = {};
        bufferInfo.ByteWidth = sizeof(vertices);
        bufferInfo.Usage = D3D11_USAGE_IMMUTABLE;
        bufferInfo.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA subresourceData = {};
        subresourceData.pSysMem = vertices;

        THROW_IF_FAILED(m_Device->CreateBuffer(&bufferInfo, &subresourceData, &m_VertexBuffer));
    }

    void D3D11::Renderer::CreateSwapchainResources()
    {
        m_FramePackets.Reserve(m_FramesInFlight);
        for(size_t i = 0; i < m_FramesInFlight; i++)
        {
            FramePacketData framePacketData;

            THROW_IF_FAILED(m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&framePacketData.BackBuffer)));
            THROW_IF_FAILED(m_Device->CreateRenderTargetView(framePacketData.BackBuffer.Get(), nullptr, &framePacketData.BackBufferRTV));

            m_FramePackets.PushBack(framePacketData);
        }
    }

    void D3D11::Renderer::DestroySwapchainResources()
    {
        for(auto [BackBuffer, BackBufferRTV] : m_FramePackets)
        {
            BackBufferRTV.Reset();
            BackBuffer.Reset();
        }
        m_FramePackets.Clear();
    }

    bool D3D11::Renderer::OnEvent(const EventType type, const EventData data)
    {
        switch(type)
        {
        case EventType::WINDOW_RESIZE:
            if(m_WindowID == data.WindowSize.ID)
            {
                m_Context->Flush();

                DestroySwapchainResources();
                THROW_IF_FAILED(m_SwapChain->ResizeBuffers(0, data.WindowSize.Width, data.WindowSize.Height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
                CreateSwapchainResources();
            }
        default:
            break;
        }

        return true;
    }

    D3D11::Renderer::~Renderer()
    {
        m_Context->Flush();
        DestroySwapchainResources();
        m_SwapChain.Reset();
        m_Factory.Reset();
        m_Context.Reset();
#ifdef NGN_DEBUG
        // 0x2 = D3D11_RLDO_DETAIL
        // 0x4 = D3D11_RLDO_IGNORE_INTERNAL (not present in MinGW headers)
        const HRESULT hr = m_Debug->ReportLiveDeviceObjects(static_cast<D3D11_RLDO_FLAGS>(0x2 | 0x4));
        if(FAILED(hr))
        {
            m_Logger.Error() << "Failed to report live device objects: " << GetErrorMessage(hr) << Logger::EndLine;
        }
        m_Debug.Reset();
#endif
        m_Device.Reset();
    }

    FramePacket D3D11::Renderer::StartFrame(FrameData& frameData)
    {
        const D3D11_VIEWPORT viewport = {
            .TopLeftX = 0,
            .TopLeftY = 0,
            .Width = static_cast<float>(m_Width),
            .Height = static_cast<float>(m_Height),
            .MinDepth = 0.0f,
            .MaxDepth = 1.0f
        };

        constexpr float clearColor[] = { 0.1f, 0.1f, 0.1f, 1.0f };

        m_Context->ClearRenderTargetView(m_FramePackets[m_CurrentFrame].BackBufferRTV.Get(), clearColor);
        m_Context->RSSetViewports(1, &viewport);
        m_Context->OMSetRenderTargets(1, m_FramePackets[m_CurrentFrame].BackBufferRTV.GetAddressOf(), nullptr);
        m_Context->IASetInputLayout(m_InputLayout.Get());

        const UINT stride = sizeof(Vertex);
        const UINT offset = 0;
        m_Context->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
        m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_ShaderProgram->Bind(*this);
        m_Context->Draw(3, 0);

        return m_CurrentFrame;
    }

    void D3D11::Renderer::EndFrame(FramePacket)
    {
        m_CurrentFrame = (m_CurrentFrame + 1) % m_FramesInFlight;

        const HRESULT hr = m_SwapChain->Present(m_VSync ? 1 : 0, 0);
        if(FAILED(hr))
        {
            m_Logger.Error() << "Failed to present swapchain" << GetErrorMessage(hr) << Logger::EndLine;
        }
    }

    void D3D11::Renderer::SetVSync(const bool enabled)
    {
        m_VSync = enabled;
    }
}
