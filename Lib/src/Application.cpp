//
// Created by Killian on 28/11/2023.
//
#include "Application.h"


#include "../public/Configuration.h"

namespace NGN
{
    struct ApplicationInner
    {
    };

    Application::Application(const List<String>& args)
        : m_Configuration(args) // TODO: Read from config file if it exists
        , m_Logger(m_Configuration.LogLevel)
        , m_Input(m_EventManager)
    {
        m_Logger.Info() << "Application created with configuration: " << m_Configuration << Logger::EndLine;
    }

    Application::~Application()
    {
        delete static_cast<ApplicationInner*>(m_Inner);
    }


    void Application::Run()
    {
        m_Logger.Info() << "Application running" << Logger::EndLine;

        while(!m_ShouldExit)
        {
            OnUpdate();
        }
    }

    void Application::Exit()
    {
        m_ShouldExit = true;
    }

}
