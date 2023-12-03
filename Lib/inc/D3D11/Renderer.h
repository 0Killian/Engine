//
// Created by Killian on 02/12/2023.
//
#pragma once

#include <memory>
#include <Renderer.h>

#include "Events.h"

#include <wrl.h>
#include <d3d11.h>
#include <dxgi1_3.h>

#include "../../public/Configuration.h"

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

        void CreateSwapchainResources();
        void DestroySwapchainResources();
    };
}
