//
// Created by Killian on 28/11/2023.
//
#pragma once

namespace NGN
{
    class Application
    {
    public:
        Application();
        virtual ~Application() = default;

        void Run();
    };
}