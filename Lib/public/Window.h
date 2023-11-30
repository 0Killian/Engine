//
// Created by Killian on 30/11/2023.
//
#pragma once
#include <cstdint>

#include "Platform.h"
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

        explicit Window(Specification spec);
        ~Window();

        void PollEvents();

    private:
        void* m_Inner = nullptr;
    };
}
