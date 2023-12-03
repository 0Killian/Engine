//
// Created by Killian on 29/11/2023.
//
#include "Events.h"

#include <ranges>

#include "Logger.h"
#include "Containers/HashMap.h"

namespace NGN
{
    EventManager::EventManager(Logger& logger)
        : m_Logger(logger)
    {
        for(auto type = static_cast<EventType>(0); type < EventType::_COUNT; type = static_cast<EventType>(static_cast<int>(type) + 1))
        {
            m_ListenerMap[type] = List<size_t>();
        }
    }

    size_t EventManager::AddListener(EventListener* listener, const List<EventType>& types)
    {
        size_t id;
        if (m_NextFree == -1)
        {
            id = m_Listeners.Size();
            m_Listeners.PushBack(listener);
        }
        else
        {
            id = m_NextFree;
            m_NextFree = -1;
            for(size_t i = id + 1; i < m_Listeners.Size(); ++i)
            {
                if (m_Listeners[i] == nullptr)
                {
                    m_NextFree = static_cast<ssize_t>(i);
                    break;
                }
            }
        }

        for (EventType type : types)
        {
            m_ListenerMap[type].PushBack(id);
        }

        return id;
    }

    void EventManager::RemoveListener(const size_t id)
    {
        if(id >= m_Listeners.Size())
            return;

        if(m_NextFree == -1 || id < m_NextFree)
        {
            m_NextFree = static_cast<ssize_t>(id);
        }

        m_Listeners[id] = nullptr;

        for(auto& list : std::ranges::views::values(m_ListenerMap))
        {
            for(size_t i = 0; i < list.Size(); ++i)
            {
                if(list[i] == id)
                {
                    list.Erase(i);
                    break;
                }
            }
        }
    }

    void EventManager::MoveListener(const size_t id, EventListener* listener)
    {
        if(id >= m_Listeners.Size())
            return;

        m_Listeners[id] = listener;
    }

    void EventManager::TriggerEvent(EventType type, const EventData data)
    {
        m_Logger.Debug() << "Triggering event " << static_cast<int>(type) << Logger::EndLine;

        for(const size_t id : m_ListenerMap[type])
        {
            if(m_Listeners[id] != nullptr)
            {
                if(!m_Listeners[id]->OnEvent(type, data))
                {
                    break;
                }
            }
        }
    }

    EventListener::EventListener(const List<EventType>& types, std::shared_ptr<EventManager> manager)
        : m_Manager(std::move(manager))
    {
        m_Id = m_Manager->AddListener(this, types);
    }

    EventListener::~EventListener()
    {
        if(m_Manager != nullptr)
            m_Manager->RemoveListener(m_Id);
    }

    EventListener::EventListener(EventListener&& other) noexcept
        : m_Manager(std::move(other.m_Manager))
        , m_Id(other.m_Id)
    {
        if(this == &other)
        {
            return;
        }

        other.m_Id = -1;

        m_Manager->MoveListener(m_Id, this);
    }

    EventListener& EventListener::operator=(EventListener&& other) noexcept
    {
        if(this == &other)
        {
            return *this;
        }

        m_Id = other.m_Id;
        m_Manager = std::move(other.m_Manager);

        other.m_Id = -1;

        m_Manager->MoveListener(m_Id, this);

        return *this;
    }
}
