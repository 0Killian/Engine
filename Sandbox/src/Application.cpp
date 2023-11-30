//
// Created by Killian on 27/11/2023.
//

#include <optional>
#include <Window.h>

#include <utility>

#include "Engine.h"

class Sandbox;

class WindowListener final : public NGN::EventListener
{
public:
    WindowListener(std::shared_ptr<NGN::EventManager> eventManager, std::optional<NGN::Window>& window1, std::optional<NGN::Window>& window2)
        : EventListener({
            NGN::EventType::WINDOW_CLOSE,
        }, std::move(eventManager))
        , m_Window1(window1)
        , m_Window2(window2)
    {}

    bool OnEvent(NGN::EventType type, NGN::EventData data) override
    {
        if(m_Window1.has_value() && data.WindowID == m_Window1->GetID())
        {
            m_Window1.reset();
        }
        else if(m_Window2.has_value() && data.WindowID == m_Window2->GetID())
        {
            m_Window2.reset();
        }

        return false;
    }

private:
    std::optional<NGN::Window>& m_Window1;
    std::optional<NGN::Window>& m_Window2;
};

class Sandbox final : public NGN::Application
{
public:
    explicit Sandbox(const NGN::List<NGN::String>& args)
        : Application(args)
        , m_Window1(NGN::Window(NGN::Window::Specification { .Title = "Window 1", .Width = 800, .Height = 600 }, m_EventManager))
        , m_Window2(NGN::Window(NGN::Window::Specification { .Title = "Window 2", .Width = 400, .Height = 600 }, m_EventManager))
        , m_WindowListener(m_EventManager, m_Window1, m_Window2)
    {
        m_Logger.Info() << "Sandbox Created" << NGN::Logger::EndLine;
    }

    void OnUpdate() override
    {
        if(m_Window1)
            m_Window1->PollEvents();

        if(m_Window2)
            m_Window2->PollEvents();

        if(!m_Window1 && !m_Window2)
            Exit();
    }

private:
    std::optional<NGN::Window> m_Window1;
    std::optional<NGN::Window> m_Window2;

    WindowListener m_WindowListener;

    friend WindowListener;
};

NGN_ENTRY_POINT(Sandbox)