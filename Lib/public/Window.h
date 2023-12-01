//
// Created by Killian on 30/11/2023.
//
#pragma once
#include <cstdint>
#include <Events.h>

#include "Containers/String.h"

namespace NGN
{
    class Window
    {
    public:
        struct Specification
        {
            String Title;
            uint32_t Width;
            uint32_t Height;
        };

        explicit Window(Specification spec, std::shared_ptr<EventManager>, Logger& logger);
        ~Window();

        Window(Window&&) noexcept;
        Window& operator=(Window&&) noexcept;

        Window(Window&) = delete;
        Window& operator=(Window&) = delete;

        void PollEvents();
        [[nodiscard]] uint64_t GetID() const;

    private:
        void* m_Inner = nullptr;
    };
}
