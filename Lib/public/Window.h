//
// Created by Killian on 30/11/2023.
//
#pragma once
#include <cstdint>

#include "Events.h"
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

        explicit Window(const Specification& spec);
        ~Window();

        Window(Window&&) noexcept;
        Window& operator=(Window&&) noexcept;

        Window(Window&) = delete;
        Window& operator=(Window&) = delete;

        void DisableEvents();
        void EnableEvents();

        void PollEvents() const;
        [[nodiscard]] uint64_t GetID() const;

        void CreateRenderer();
        [[nodiscard]] Renderer& GetRenderer() const;

        [[nodiscard]] bool ShouldClose() const;
        [[nodiscard]] size_t GetWidth() const;
        [[nodiscard]] size_t GetHeight() const;

    private:
        void* m_Inner = nullptr;
    };
}
