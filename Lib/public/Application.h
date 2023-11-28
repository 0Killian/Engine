//
// Created by Killian on 28/11/2023.
//
#pragma once
#include "Containers/String.h"

namespace NGN
{
    class Application
    {
    public:
        explicit Application(const List<String>& args);
        virtual ~Application() = default;

        void Run();
    };
}
