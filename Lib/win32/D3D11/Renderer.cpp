//
// Created by Killian on 02/12/2023.
//
#include "D3D11/imgui_impl_dx11.h"
#include "D3D11/Renderer.h"

#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <d3d11sdklayers.h>
#include <stdexcept>

#include "D3D11/Utils.h"

namespace NGN
{
    D3D11::Renderer::Renderer(
        void* hwnd,
        const size_t width,
        const size_t height,
        const size_t id)
        : EventListener({
            EventType::WINDOW_RESIZE
        })
        , m_WindowID(id)
        , m_VSync(Configuration::Get().VSync)
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

        ImGui_ImplDX11_Init(m_Device.Get(), m_Context.Get());
    }

    void D3D11::Renderer::CreateSwapchainResources()
    {
        m_FramePackets.Reserve(m_FramesInFlight);
        for(size_t i = 0; i < m_FramesInFlight; i++)
        {
            FramePacketData framePacketData;

            THROW_IF_FAILED(m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&framePacketData.BackBuffer)));
            THROW_IF_FAILED(m_Device->CreateRenderTargetView(framePacketData.BackBuffer.Get(), nullptr, &framePacketData.BackBufferRTV));
            
            D3D11_TEXTURE2D_DESC desc;
			framePacketData.BackBuffer->GetDesc(&desc);

            desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;
            desc.Usage = D3D11_USAGE_DEFAULT;
            desc.MipLevels = 1;
            desc.ArraySize = 1;
            desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            desc.SampleDesc.Count = 1;
            desc.MiscFlags = 0;

            THROW_IF_FAILED(m_Device->CreateTexture2D(&desc, nullptr, &framePacketData.RenderTexture));

            D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
            rtvDesc.Format = desc.Format;
            rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            rtvDesc.Texture2D.MipSlice = 0;

            THROW_IF_FAILED(m_Device->CreateRenderTargetView(framePacketData.RenderTexture.Get(), &rtvDesc, &framePacketData.RenderTextureRTV));

            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
            srvDesc.Format = desc.Format;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MipLevels = desc.MipLevels;
            srvDesc.Texture2D.MostDetailedMip = 0;

            THROW_IF_FAILED(m_Device->CreateShaderResourceView(framePacketData.RenderTexture.Get(), &srvDesc, &framePacketData.RenderTextureSRV));

            // Depth stencil
            D3D11_TEXTURE2D_DESC depthStencilDesc = {};
            depthStencilDesc.Width = desc.Width;
            depthStencilDesc.Height = desc.Height;
            depthStencilDesc.MipLevels = 1;
            depthStencilDesc.ArraySize = 1;
            depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
            depthStencilDesc.SampleDesc.Count = 1;
            depthStencilDesc.SampleDesc.Quality = 0;
            depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
            depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
            depthStencilDesc.CPUAccessFlags = 0;
            depthStencilDesc.MiscFlags = 0;

            THROW_IF_FAILED(m_Device->CreateTexture2D(&depthStencilDesc, nullptr, &framePacketData.DepthStencilBuffer));

            THROW_IF_FAILED(m_Device->CreateDepthStencilView(framePacketData.DepthStencilBuffer.Get(), nullptr, &framePacketData.DepthStencilView));

            D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc = {};
            depthStencilStateDesc.DepthEnable = true;
            depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
            depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

			THROW_IF_FAILED(m_Device->CreateDepthStencilState(&depthStencilStateDesc, &framePacketData.DepthStencilState));

            D3D11_RASTERIZER_DESC rasterizerDesc = {};
            rasterizerDesc.FillMode = D3D11_FILL_SOLID;
            rasterizerDesc.CullMode = D3D11_CULL_BACK;
            rasterizerDesc.FrontCounterClockwise = true;
            rasterizerDesc.DepthBias = 0;
            rasterizerDesc.DepthBiasClamp = 0.0f;
            rasterizerDesc.SlopeScaledDepthBias = 0.0f;
            rasterizerDesc.DepthClipEnable = true;
            rasterizerDesc.ScissorEnable = false;
            rasterizerDesc.MultisampleEnable = false;
            rasterizerDesc.AntialiasedLineEnable = false;

            THROW_IF_FAILED(m_Device->CreateRasterizerState(&rasterizerDesc, &framePacketData.RasterizerState));

            m_FramePackets.PushBack(framePacketData);
        }
    }

    void D3D11::Renderer::DestroySwapchainResources()
    {
        for(auto& packet : m_FramePackets)
        {
            packet.BackBufferRTV.Reset();
            packet.RenderTexture.Reset();
            packet.RenderTextureSRV.Reset();
            packet.BackBuffer.Reset();
            packet.RenderTextureRTV.Reset();
            packet.DepthStencilBuffer.Reset();
			packet.DepthStencilView.Reset();
            packet.DepthStencilState.Reset();
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
            break;

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
            Logger::Error() << "Failed to report live device objects: " << GetErrorMessage(hr) << Logger::EndLine;
        }
        m_Debug.Reset();
#endif
        m_Device.Reset();
    }

    FramePacket D3D11::Renderer::StartFrame(FrameData& frameData)
    {
        ImGui_ImplDX11_NewFrame();
        const D3D11_VIEWPORT viewport = {
            .TopLeftX = frameData.Viewport.X,
            .TopLeftY = frameData.Viewport.Y,
            .Width = frameData.Viewport.Width,
            .Height = frameData.Viewport.Height,
            .MinDepth = 0.0f,
            .MaxDepth = 1.0f
        };

        constexpr float clearColor[] = { 0.1f, 0.1f, 0.1f, 1.0f };

        m_Context->RSSetViewports(1, &viewport);

        if(m_RenderMode == RenderMode::ToScreen)
		{
			m_Context->ClearRenderTargetView(m_FramePackets[m_CurrentFrame].BackBufferRTV.Get(), clearColor);
			m_Context->OMSetRenderTargets(1, m_FramePackets[m_CurrentFrame].BackBufferRTV.GetAddressOf(), m_FramePackets[m_CurrentFrame].DepthStencilView.Get());
		}
		else if(m_RenderMode == RenderMode::ToTexture)
		{
			m_Context->ClearRenderTargetView(m_FramePackets[m_CurrentFrame].RenderTextureRTV.Get(), clearColor);
			m_Context->OMSetRenderTargets(1, m_FramePackets[m_CurrentFrame].RenderTextureRTV.GetAddressOf(), m_FramePackets[m_CurrentFrame].DepthStencilView.Get());
		}

        m_Context->ClearDepthStencilView(m_FramePackets[m_CurrentFrame].DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        m_Context->OMSetDepthStencilState(m_FramePackets[m_CurrentFrame].DepthStencilState.Get(), 1);
        m_Context->RSSetState(m_FramePackets[m_CurrentFrame].RasterizerState.Get());

        for (auto& pipeline : m_Pipelines)
        {
            pipeline.UpdateConstantBuffer(frameData.ConstantBuffer);
		}

        return m_CurrentFrame;
    }

    void D3D11::Renderer::EndFrame(FramePacket)
    {
        m_CurrentFrame = (m_CurrentFrame + 1) % m_FramesInFlight;

        for (auto& pipeline : m_Pipelines)
        {
            pipeline.Render();
        }

        if (m_RenderMode == RenderMode::ToTexture)
        {
            float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
            m_Context->ClearRenderTargetView(m_FramePackets[m_CurrentFrame].BackBufferRTV.Get(), clearColor);
            m_Context->OMSetRenderTargets(1, m_FramePackets[m_CurrentFrame].BackBufferRTV.GetAddressOf(), nullptr);
        }

        ImGui::Render();

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        HRESULT hr = m_SwapChain->Present(m_VSync ? 1 : 0, 0);
        if(hr == DXGI_ERROR_DEVICE_REMOVED)
        {
            hr = m_Device->GetDeviceRemovedReason();
            Logger::Error() << "Device removed: " << ToHex(hr) << Logger::EndLine;
            __debugbreak();
        }
        if(FAILED(hr))
        {
            Logger::Error() << "Failed to present swapchain" << GetErrorMessage(hr) << Logger::EndLine;
        }
    }

    void D3D11::Renderer::SetVSync(const bool enabled)
    {
        m_VSync = enabled;
    }

    // Resources
    size_t D3D11::Renderer::CreatePipeline(const String& vertexSource, const String& pixelSource)
    {
        m_Pipelines.EmplaceBack(vertexSource, pixelSource);
        m_PipelineMap.Insert(m_NextPipelineID, m_Pipelines.Size() - 1);
        return m_NextPipelineID++;
    }

    size_t D3D11::Renderer::CreatePipeline(const List<uint8_t>& compiledVertex, const List<uint8_t>& compiledPixel)
    {
        m_Pipelines.EmplaceBack(compiledVertex, compiledPixel);
        m_PipelineMap.Insert(m_NextPipelineID, m_Pipelines.Size() - 1);
        return m_NextPipelineID++;
    }

    void D3D11::Renderer::UpdateMeshInPipeline(size_t RendererID, size_t meshID, String name, class Mesh& meshData)
    {
        if(!m_PipelineMap.ContainsKey(RendererID))
		{
			throw std::runtime_error("Invalid RendererID");
		}

		m_Pipelines[m_PipelineMap[RendererID]].UpdateMesh(meshID, name, meshData);
    }

    size_t D3D11::Renderer::AddMeshInstanceInPipeline(size_t RendererID, size_t meshID, InstanceBuffer buffer)
    {
        if (!m_PipelineMap.ContainsKey(RendererID))
        {
            throw std::runtime_error("Invalid RendererID");
        }

        return m_Pipelines[m_PipelineMap[RendererID]].AddInstance(meshID, buffer);
    }

    void D3D11::Renderer::UpdateMeshInstanceInPipeline(size_t RendererID, size_t meshID, size_t instanceID, InstanceBuffer buffer)
    {
        if (!m_PipelineMap.ContainsKey(RendererID))
        {
			throw std::runtime_error("Invalid RendererID");
		}

		m_Pipelines[m_PipelineMap[RendererID]].UpdateInstance(meshID, instanceID, buffer);
    }

    void D3D11::Renderer::RemoveMeshInstanceInPipeline(size_t RendererID, size_t meshID, size_t instanceID)
    {
        if (!m_PipelineMap.ContainsKey(RendererID))
        {
            throw std::runtime_error("Invalid RendererID");
        }

        m_Pipelines[m_PipelineMap[RendererID]].RemoveInstance(meshID, instanceID);
    }
}
