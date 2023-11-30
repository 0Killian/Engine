//
// Created by Killian on 27/11/2023.
//

#include <Window.h>

#include "Engine.h"

class Sandbox final : public NGN::Application
{
public:
    explicit Sandbox(const NGN::List<NGN::String>& args)
        : Application(args)
        , m_Window1(NGN::Window::Specification { .Title = "Window 1", .Width = 800, .Height = 600 })
        , m_Window2(NGN::Window::Specification { .Title = "Window 2", .Width = 400, .Height = 600 })
    {
        m_Logger.Info() << "Sandbox Created" << NGN::Logger::EndLine;
    }

    void OnUpdate() override
    {
        m_Window1.PollEvents();
        m_Window2.PollEvents();
    }

private:
    NGN::Window m_Window1;
    NGN::Window m_Window2;
};

NGN_ENTRY_POINT(Sandbox)