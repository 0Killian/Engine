//
// Created by Killian on 28/11/2023.
//
#include "Application.h"

#include <iostream>

#include "Configuration.h"

namespace NGN
{
    struct ApplicationInner
    {
        Configuration m_Configuration;
    };

    Application::Application(const List<String>& args)
    {
        std::cout << "Application started. Args: " << args << std::endl;
        const auto inner = new ApplicationInner();
        m_Inner = inner;
        inner->m_Configuration.ParseArgs(args);
        // TODO: Read from config file if it exists

        std::cout << "Configuration: " << inner->m_Configuration << std::endl;
    }

    Application::~Application()
    {
        delete static_cast<ApplicationInner*>(m_Inner);
    }


    void Application::Run()
    {
        std::cout << "Hello, World!" << std::endl;
    }
}
