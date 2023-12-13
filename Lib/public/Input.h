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
        explicit Input();

        [[nodiscard]] static bool IsKeyPressed(Key key);
        [[nodiscard]] static bool IsMouseButtonPressed(MouseButton button);
        [[nodiscard]] static float GetMouseX();
        [[nodiscard]] static float GetMouseY();
        [[nodiscard]] static float GetMouseScrollX();
        [[nodiscard]] static float GetMouseScrollY();
        [[nodiscard]] static float GetMouseDeltaX();
        [[nodiscard]] static float GetMouseDeltaY();

        static void StartTyping();
        static void StopTyping();
        [[nodiscard]] static String& GetTextBuffer();

        static Input& Get();

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
