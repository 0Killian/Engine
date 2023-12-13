//
// Created by Killian on 02/12/2023.
//
#pragma once

#include <memory>
#include <Renderer.h>

#include <wrl.h>
#include <d3d11.h>
#include <dxgi1_3.h>

#include "win.h"
#include "Events.h"
#include "Pipeline.h"
#include "Configuration.h"
#include "RenderAPI.h"

namespace NGN
{
	class ResourceManager;
    class VertexBuffer;
}

namespace NGN::D3D11
{
    class Renderer final : public NGN::Renderer, public EventListener
    {
    public:
        Renderer(void* hwnd, size_t width, size_t height, size_t id);
        ~Renderer() override;

        Renderer(Renderer&&) = default;
        Renderer& operator=(Renderer&&) noexcept = delete;
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        RenderAPI GetAPI() const override { return RenderAPI::D3D11; }

        FramePacket StartFrame(FrameData& frameData) override;
        void EndFrame(FramePacket packet) override;
        
        size_t CreatePipeline(const String& vertexSource, const String& pixelSource) override;
        size_t CreatePipeline(const List<uint8_t>& compiledVertex, const List<uint8_t>& compiledPixel) override;
        
        void UpdateMeshInPipeline(size_t RendererID, size_t meshID, String name, Mesh& mesh) override;

        size_t AddMeshInstanceInPipeline(size_t RendererID, size_t meshID, InstanceBuffer buffer) override;
        void UpdateMeshInstanceInPipeline(size_t RendererID, size_t meshID, size_t instanceID, InstanceBuffer buffer) override;

        void SetVSync(bool enabled);

        [[nodiscard]] inline ID3D11Device* GetDevice() const { return m_Device.Get(); }
        [[nodiscard]] inline ID3D11DeviceContext* GetContext() const { return m_Context.Get(); }

    protected:
        bool OnEvent(EventType type, EventData data) override;

    private:
        template <typename T>
        using ComPtr = Microsoft::WRL::ComPtr<T>;

        ComPtr<ID3D11Device> m_Device = nullptr;
        ComPtr<ID3D11DeviceContext> m_Context = nullptr;
        ComPtr<IDXGIFactory2> m_Factory = nullptr;
        ComPtr<IDXGISwapChain1> m_SwapChain = nullptr;

#ifdef NGN_DEBUG
        ComPtr<ID3D11Debug> m_Debug = nullptr;
#endif

        size_t m_WindowID = 0;
        bool m_VSync = false;

        struct FramePacketData
        {
            ComPtr<ID3D11Texture2D> BackBuffer;
            ComPtr<ID3D11RenderTargetView> BackBufferRTV;
        };

        List<FramePacketData> m_FramePackets;
        size_t m_FramesInFlight = 0;
        size_t m_CurrentFrame = 0;

        size_t m_Width = 0;
        size_t m_Height = 0;

        // Resources
        List<Pipeline> m_Pipelines;
        HashMap<size_t, size_t> m_PipelineMap;
        size_t m_NextPipelineID = 0;

        void CreateSwapchainResources();
        void DestroySwapchainResources();
    };
}
