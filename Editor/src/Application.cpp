//
// Created by Killian on 27/11/2023.
//

#include "Engine.h"

class Editor final : public NGN::Application
{
public:
    explicit Editor(const NGN::List<NGN::String>& args)
        : Application(args)
    {
        m_Logger.Info() << "Editor Created" << NGN::Logger::EndLine;
    }

    void OnUpdate() override
    {
        m_EventManager->TriggerEvent(NGN::EventType::DEBUG1, { .Debug = nullptr });
        m_EventManager->TriggerEvent(NGN::EventType::DEBUG2, { .Debug = nullptr });
        m_EventManager->TriggerEvent(NGN::EventType::DEBUG3, { .Debug = nullptr });
    }
};

NGN_ENTRY_POINT(Editor)