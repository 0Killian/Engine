//
// Created by Killian on 02/12/2023.
//
#include "D3D11/Renderer.h"

#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <d3d11sdklayers.h>

namespace NGN
{
    D3D11Renderer::D3D11Renderer(void* hwnd, Logger& logger, std::shared_ptr<EventManager> eventManager, Configuration& config, size_t width, size_t height, size_t id)
        : EventListener({
            EventType::WINDOW_RESIZE
        }, eventManager)
        , m_Logger(logger)
        , m_EventManager(std::move(eventManager))
        , m_WindowID(id)
        , m_VSync(config.VSync)
    {
        if(FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&m_Factory))))
        {
            throw std::runtime_error("Failed to create DXGI Factory");
        }

        constexpr auto featureLevel = D3D_FEATURE_LEVEL_11_1;
        UINT flags = 0;//D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef NGN_DEBUG
        flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        if(FAILED(D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            flags,
            &featureLevel,
            1,
            D3D11_SDK_VERSION,
            &m_Device,
            nullptr,
            &m_Context)))
        {
            throw std::runtime_error("Failed to create D3D11 Device");
        }

#ifdef NGN_DEBUG
        if(FAILED(m_Device.As(&m_Debug)))
        {
            throw std::runtime_error("Failed to get D3D11 Debug interface");
        }
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

        HRESULT hr = m_Factory->CreateSwapChainForHwnd(
            m_Device.Get(),
            static_cast<HWND>(hwnd),
            &swapChainDesc,
            &swapChainFullscreenDesc,
            nullptr,
            &m_SwapChain);
        if(FAILED(hr))
        {
            throw std::runtime_error("Failed to create DXGI SwapChain: " + std::to_string(hr));
        }

        CreateSwapchainResources();
    }

    void D3D11Renderer::CreateSwapchainResources()
    {
        m_FramePackets.Reserve(m_FramesInFlight);
        for(size_t i = 0; i < m_FramesInFlight; i++)
        {
            FramePacketData framePacketData;
            if(FAILED(m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&framePacketData.BackBuffer))))
            {
                throw std::runtime_error("Failed to get DXGI SwapChain back buffer");
            }

            if(FAILED(m_Device->CreateRenderTargetView(framePacketData.BackBuffer.Get(), nullptr, &framePacketData.BackBufferRTV)))
            {
                throw std::runtime_error("Failed to create D3D11 RenderTargetView");
            }

            m_FramePackets.PushBack(framePacketData);
        }
    }

    void D3D11Renderer::DestroySwapchainResources()
    {
        for(auto framePacket : m_FramePackets)
        {
            framePacket.BackBufferRTV.Reset();
            framePacket.BackBuffer.Reset();
        }
        m_FramePackets.Clear();
    }

    bool D3D11Renderer::OnEvent(EventType type, EventData data)
    {
        switch(type)
        {
        case EventType::WINDOW_RESIZE:
            if(m_WindowID == data.WindowSize.ID)
            {
                m_Context->Flush();

                DestroySwapchainResources();
                if(FAILED(m_SwapChain->ResizeBuffers(0, data.WindowSize.Width, data.WindowSize.Height, DXGI_FORMAT_R8G8B8A8_UNORM, 0)))
                {
                    throw std::runtime_error("Failed to resize DXGI SwapChain buffers");
                }
                CreateSwapchainResources();
            }
        default:
            break;
        }

        return true;
    }

    D3D11Renderer::~D3D11Renderer()
    {
        m_Context->Flush();
        DestroySwapchainResources();
        m_SwapChain.Reset();
        m_Factory.Reset();
        m_Context.Reset();
#ifdef NGN_DEBUG
        if(FAILED(m_Debug->ReportLiveDeviceObjects(static_cast<D3D11_RLDO_FLAGS>(0x2 | 0x4))))
        {
            throw std::runtime_error("Failed to report live D3D11 objects");
        }
        m_Debug.Reset();
#endif
        m_Device.Reset();
    }

    FramePacket D3D11Renderer::StartFrame(FrameData& frameData)
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

        return m_CurrentFrame;
    }

    void D3D11Renderer::EndFrame(FramePacket&& packet)
    {
        m_CurrentFrame = (m_CurrentFrame + 1) % m_FramesInFlight;
        if(FAILED(m_SwapChain->Present(1, 0)))
        {
            throw std::runtime_error("Failed to present DXGI SwapChain");
        }
    }
}
