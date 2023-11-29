//
// Created by Killian on 27/11/2023.
//

#include "Engine.h"

class DebugListener : public NGN::EventListener
{
public:
    DebugListener(const std::shared_ptr<NGN::EventManager>& manager, NGN::Logger& logger)
        : EventListener({
            NGN::EventType::DEBUG1,
            NGN::EventType::DEBUG3
        }, manager), m_Logger(logger)
    {}

    bool OnEvent(NGN::EventType type, NGN::EventData data) override
    {
        m_Logger.Info() << "Event " << static_cast<int>(type) << "Triggered!" << NGN::Logger::EndLine;

        return true;
    }

private:
    NGN::Logger& m_Logger;
};

class Sandbox final : public NGN::Application
{
public:
    explicit Sandbox(const NGN::List<NGN::String>& args)
        : Application(args), m_Listener(m_EventManager, m_Logger)
    {
        m_Logger.Info() << "Sandbox Created" << NGN::Logger::EndLine;
    }

    void OnUpdate() override
    {
        m_EventManager->TriggerEvent(NGN::EventType::DEBUG1, { .Debug = nullptr });
        m_EventManager->TriggerEvent(NGN::EventType::DEBUG2, { .Debug = nullptr });
        m_EventManager->TriggerEvent(NGN::EventType::DEBUG3, { .Debug = nullptr });
    }

private:
    DebugListener m_Listener;
};

NGN_ENTRY_POINT(Sandbox)