//
// Created by Killian on 30/11/2023.
//
#pragma once
#include <cstdint>
#include <Events.h>

#include "Configuration.h"
#include "RenderAPI.h"
#include "Renderer.h"
#include "Containers/String.h"

namespace NGN
{
    class Window
    {
    public:
        struct Specification
        {
            String Title = "NGN Application";
            uint32_t Width = 1280;
            uint32_t Height = 720;
            RenderAPI API = RenderAPI::NONE;
        };

        explicit Window(const Specification& spec, const std::shared_ptr<EventManager>&, Logger& logger, Configuration& config);
        ~Window();

        Window(Window&&) noexcept;
        Window& operator=(Window&&) noexcept;

        Window(Window&) = delete;
        Window& operator=(Window&) = delete;

        void PollEvents() const;
        [[nodiscard]] uint64_t GetID() const;

        [[nodiscard]] Renderer& GetRenderer() const;

    private:
        void* m_Inner = nullptr;
    };
}
