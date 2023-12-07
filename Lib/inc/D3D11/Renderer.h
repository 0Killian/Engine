//
// Created by Killian on 02/12/2023.
//
#pragma once

#include <memory>
#include <Renderer.h>

#include "win.h"
#include "Events.h"

#include <wrl.h>
#include <d3d11.h>
#include <dxgi1_3.h>

#include "ShaderProgram.h"
#include "Configuration.h"

namespace NGN::D3D11
{
    class Renderer final : public NGN::Renderer, public EventListener
    {
    public:
        Renderer(void* hwnd, Logger& logger, std::shared_ptr<EventManager> eventManager, const Configuration& config, size_t width, size_t height, size_t id);
        ~Renderer() override;

        Renderer(Renderer&&) = default;
        Renderer& operator=(Renderer&&) noexcept = delete;
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        FramePacket StartFrame(FrameData& frameData) override;
        void EndFrame(FramePacket packet) override;

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
        ComPtr<ID3D11InputLayout> m_InputLayout = nullptr;

#ifdef NGN_DEBUG
        ComPtr<ID3D11Debug> m_Debug = nullptr;
#endif

        Logger& m_Logger;
        std::shared_ptr<EventManager> m_EventManager;
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

        std::unique_ptr<ShaderProgram> m_ShaderProgram = nullptr;
        ComPtr<ID3D11Buffer> m_VertexBuffer = nullptr;

        void CreateSwapchainResources();
        void DestroySwapchainResources();
    };
}
