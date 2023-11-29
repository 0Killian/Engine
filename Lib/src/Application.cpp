//
// Created by Killian on 28/11/2023.
//
#include "Application.h"

#include <iostream>
#include <thread>

#include "Configuration.h"

namespace NGN
{
    struct ApplicationInner
    {
        Configuration m_Configuration;
    };

    Application::Application(const List<String>& args)
        : m_Inner(new ApplicationInner {
            Configuration(args) // TODO: Read from config file if it exists
        }),
        m_Logger(static_cast<ApplicationInner*>(m_Inner)->m_Configuration.log_level)
    {
        m_Logger.Info() << "Application created with configuration: " << static_cast<ApplicationInner*>(m_Inner)->m_Configuration << Logger::EndLine;
    }

    Application::~Application()
    {
        delete static_cast<ApplicationInner*>(m_Inner);
    }


    void Application::Run()
    {
        m_Logger.Info() << "Application running" << Logger::EndLine;

        while(true)
        {
            OnUpdate();
        }
    }
}
