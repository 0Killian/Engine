//
// Created by Killian on 28/11/2023.
//
#pragma once
#include <memory>

#include "Containers/String.h"

namespace NGN
{
    class Application
    {
    public:
        explicit Application(const List<String>& args);
        virtual ~Application();

        void Run();

    private:
        void* m_Inner = nullptr;
    };
}
