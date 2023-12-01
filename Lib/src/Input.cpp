//
// Created by Killian on 01/12/2023.
//
#include "Input.h"

namespace NGN
{
    Input::Input(std::shared_ptr<EventManager> eventManager)
        : EventListener({
            EventType::KEY_PRESSED,
            EventType::KEY_RELEASED,
            EventType::MOUSE_BUTTON_PRESSED,
            EventType::MOUSE_BUTTON_RELEASED,
            EventType::MOUSE_MOVED,
            EventType::MOUSE_SCROLLED,
            // EventType::FRAME_STARTED
        }, std::move(eventManager))
        , m_TextBuffer("")
    {}

    bool Input::IsKeyPressed(Key key)
    {
        return m_Keys[key];
    }

    bool Input::IsMouseButtonPressed(MouseButton button)
    {
        return m_MouseButtons[button];
    }

    float Input::GetMouseX() const
    {
        return m_MouseX;
    }

    float Input::GetMouseY() const
    {
        return m_MouseY;
    }

    float Input::GetMouseScrollX() const
    {
        return m_MouseScrollX;
    }

    float Input::GetMouseScrollY() const
    {
        return m_MouseScrollY;
    }

    float Input::GetMouseDeltaX() const
    {
        return m_MouseDeltaX;
    }

    float Input::GetMouseDeltaY() const
    {
        return m_MouseDeltaY;
    }

    void Input::StartTyping()
    {
        m_Typing = true;
        m_TextBuffer = "";
    }

    void Input::StopTyping()
    {
        m_Typing = false;
    }

    String& Input::GetTextBuffer()
    {
        return m_TextBuffer;
    }

    bool Input::OnEvent(EventType type, EventData data)
    {
        switch(type)
        {
        case EventType::KEY_PRESSED:
            m_Keys[data.Key] = true;
            break;

        case EventType::KEY_RELEASED:
            m_Keys[data.Key] = false;
            if(m_Typing)
            {
                // TODO: Handle text input
            }
            break;

        case EventType::MOUSE_BUTTON_PRESSED:
            m_MouseButtons[data.MouseButton] = true;
            break;

        case EventType::MOUSE_BUTTON_RELEASED:
            m_MouseButtons[data.MouseButton] = false;
            break;

        case EventType::MOUSE_MOVED:
            m_MouseX = data.MouseMoved.X;
            m_MouseY = data.MouseMoved.Y;
            m_MouseDeltaX = m_MouseX - m_OldMouseX;
            m_MouseDeltaY = m_MouseY - m_OldMouseY;
            break;

        case EventType::MOUSE_SCROLLED:
            m_MouseScrollX += data.MouseScrolled.X;
            m_MouseScrollY += data.MouseScrolled.Y;
            break;

        /*case EventType::FRAME_STARTED:
            m_MouseDeltaX = 0;
            m_MouseDeltaY = 0;
            m_OldMouseX = m_MouseX;
            m_OldMouseY = m_MouseY;
            m_MouseScrollX = 0;
            m_MouseScrollY = 0;
            break;*/

        default:
            break;
        }

        return true;
    }
}