//
// Created by Killian on 01/12/2023.
//
#include "Input.h"
#include "Application.h"

namespace NGN
{
    Input& Input::Get()
    {
		return Application::Get().GetInput();
    }

    Input::Input()
        : EventListener({
            EventType::KEY_PRESSED,
            EventType::KEY_RELEASED,
            EventType::MOUSE_BUTTON_PRESSED,
            EventType::MOUSE_BUTTON_RELEASED,
            EventType::MOUSE_MOVED,
            EventType::MOUSE_SCROLLED,
            // EventType::FRAME_STARTED
        })
    {}

    bool Input::IsKeyPressed(const Key key)
    {
        return Get().m_Keys[key];
    }

    bool Input::IsMouseButtonPressed(const MouseButton button)
    {
        return Get().m_MouseButtons[button];
    }

    float Input::GetMouseX()
    {
        return Get().m_MouseX;
    }

    float Input::GetMouseY()
    {
        return Get().m_MouseY;
    }

    float Input::GetMouseScrollX()
    {
        return Get().m_MouseScrollX;
    }

    float Input::GetMouseScrollY()
    {
        return Get().m_MouseScrollY;
    }

    float Input::GetMouseDeltaX()
    {
        return Get().m_MouseDeltaX;
    }

    float Input::GetMouseDeltaY()
    {
        return Get().m_MouseDeltaY;
    }

    void Input::StartTyping()
    {
        Get().m_Typing = true;
        Get().m_TextBuffer = "";
    }

    void Input::StopTyping()
    {
        Get().m_Typing = false;
    }

    String& Input::GetTextBuffer()
    {
        return Get().m_TextBuffer;
    }

    bool Input::OnEvent(const EventType type, const EventData data)
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