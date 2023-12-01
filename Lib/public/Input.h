//
// Created by Killian on 01/12/2023.
//
#pragma once

#include "Events.h"
#include "Containers/Bitfield.h"
#include "Containers/String.h"
#include "Key.h"
#include "MouseButton.h"

namespace NGN
{
    class Input final : public EventListener
    {
        // TODO: Support for gamepads
    public:
        explicit Input(std::shared_ptr<EventManager> eventManager);

        bool IsKeyPressed(Key key);
        bool IsMouseButtonPressed(MouseButton button);
        [[nodiscard]] float GetMouseX() const;
        [[nodiscard]] float GetMouseY() const;
        [[nodiscard]] float GetMouseScrollX() const;
        [[nodiscard]] float GetMouseScrollY() const;
        [[nodiscard]] float GetMouseDeltaX() const;
        [[nodiscard]] float GetMouseDeltaY() const;

        void StartTyping();
        void StopTyping();
        [[nodiscard]] String& GetTextBuffer();

    protected:
        bool OnEvent(EventType type, EventData data) override;

    private:
        Bitfield<Key> m_Keys = Bitfield<Key>();
        Bitfield<MouseButton> m_MouseButtons = Bitfield<MouseButton>();
        float m_MouseX = 0.0f;
        float m_MouseY = 0.0f;
        float m_MouseScrollX = 0.0f;
        float m_MouseScrollY = 0.0f;
        float m_MouseDeltaX = 0.0f;
        float m_MouseDeltaY = 0.0f;
        float m_OldMouseX = 0.0f;
        float m_OldMouseY = 0.0f;
        String m_TextBuffer = "";
        bool m_Typing = false;
    };
}
