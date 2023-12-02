//
// Created by Killian on 27/11/2023.
//

#include <optional>
#include <Window.h>

#include <utility>

#include "Engine.h"
#include "Renderer.h"

class Sandbox;

class WindowListener final : public NGN::EventListener
{
public:
    WindowListener(std::shared_ptr<NGN::EventManager> eventManager, std::optional<NGN::Window>& window1, std::optional<NGN::Window>& window2, NGN::Logger& logger)
        : EventListener({
            NGN::EventType::WINDOW_CLOSE,
            NGN::EventType::WINDOW_RESIZE,
            NGN::EventType::WINDOW_FOCUS,
            NGN::EventType::WINDOW_LOST_FOCUS,
            NGN::EventType::WINDOW_MOVED,
            NGN::EventType::KEY_PRESSED,
            NGN::EventType::KEY_RELEASED,
        }, std::move(eventManager))
        , m_Window1(window1)
        , m_Window2(window2)
        , m_Logger(logger)
    {}

    bool OnEvent(NGN::EventType type, NGN::EventData data) override
    {
        if(type == NGN::EventType::KEY_PRESSED)
            m_Logger.Info() << "Key Pressed: " << ToString(data.Key) << NGN::Logger::EndLine;
        else if(type == NGN::EventType::KEY_RELEASED)
            m_Logger.Info() << "Key Released: " << ToString(data.Key) << NGN::Logger::EndLine;
        else if(m_Window1.has_value() && data.WindowID == m_Window1->GetID())
        {
            if(type == NGN::EventType::WINDOW_CLOSE)
                m_Window1.reset();
            else if(type == NGN::EventType::WINDOW_FOCUS)
                m_Logger.Info() << "Window 1 Focused" << NGN::Logger::EndLine;
            else if(type == NGN::EventType::WINDOW_LOST_FOCUS)
                m_Logger.Info() << "Window 1 Lost Focus" << NGN::Logger::EndLine;
            else if(type == NGN::EventType::WINDOW_MOVED)
                m_Logger.Info() << "Window 1 Moved to " << data.WindowMoved.X << "x" << data.WindowMoved.Y << NGN::Logger::EndLine;
            else if(type == NGN::EventType::WINDOW_RESIZE)
                m_Logger.Info() << "Window 1 Resized to " << data.WindowSize.Width << "x" << data.WindowSize.Height << NGN::Logger::EndLine;
        }
        else if(m_Window2.has_value() && data.WindowID == m_Window2->GetID())
        {
            if(type == NGN::EventType::WINDOW_CLOSE)
                m_Window2.reset();
            else if(type == NGN::EventType::WINDOW_FOCUS)
                m_Logger.Info() << "Window 2 Focused" << NGN::Logger::EndLine;
            else if(type == NGN::EventType::WINDOW_LOST_FOCUS)
                m_Logger.Info() << "Window 2 Lost Focus" << NGN::Logger::EndLine;
            else if(type == NGN::EventType::WINDOW_MOVED)
                m_Logger.Info() << "Window 2 Moved to " << data.WindowMoved.X << "x" << data.WindowMoved.Y << NGN::Logger::EndLine;
            else if(type == NGN::EventType::WINDOW_RESIZE)
                m_Logger.Info() << "Window 2 Resized to " << data.WindowSize.Width << "x" << data.WindowSize.Height << NGN::Logger::EndLine;
        }

        return false;
    }

private:
    std::optional<NGN::Window>& m_Window1;
    std::optional<NGN::Window>& m_Window2;
    NGN::Logger& m_Logger;
};

class Sandbox final : public NGN::Application
{
public:
    explicit Sandbox(const NGN::List<NGN::String>& args)
        : Application(args)
        , m_Window1(NGN::Window(NGN::Window::Specification { .Title = "Window 1", .Width = 800, .Height = 600, .API = NGN::RenderAPI::D3D11 }, m_EventManager, m_Logger, m_Configuration))
        , m_Window2(NGN::Window(NGN::Window::Specification { .Title = "Window 2", .Width = 400, .Height = 600 }, m_EventManager, m_Logger, m_Configuration))
        , m_WindowListener(m_EventManager, m_Window1, m_Window2, m_Logger)
        , m_Renderer(&(m_Window1->GetRenderer()))
    {
        if(m_Renderer == nullptr)
            throw std::runtime_error("Renderer was not created");
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

        if(m_Window1)
        {
            NGN::FrameData data;
            NGN::FramePacket packet = m_Renderer->StartFrame(data);
            m_Renderer->EndFrame(std::move(packet));
        }
    }

private:
    std::optional<NGN::Window> m_Window1;
    std::optional<NGN::Window> m_Window2;
    NGN::Renderer* m_Renderer = nullptr;

    WindowListener m_WindowListener;

    friend WindowListener;
};

NGN_ENTRY_POINT(Sandbox)