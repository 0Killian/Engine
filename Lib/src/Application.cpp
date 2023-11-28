//
// Created by Killian on 28/11/2023.
//
#include "Application.h"

#include <iostream>

namespace NGN
{
    Application::Application(const List<String>& args)
    {
        std::cout << "Application started. Args: " << args << std::endl;
    }

    void Application::Run()
    {
        std::cout << "Hello, World!" << std::endl;
    }
}