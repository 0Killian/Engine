//
// Created by Killian on 28/11/2023.
//
#include "Application.h"

#include <iostream>

namespace NGN
{
    Application::Application()
    {
        std::cout << "Application started" << std::endl;
    }

    void Application::Run()
    {
        std::cout << "Hello, World!" << std::endl;
    }
}