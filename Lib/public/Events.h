//
// Created by Killian on 29/11/2023.
//
#pragma once
#include <memory>

#include "Containers/HashMap.h"
#include "Containers/List.h"

namespace NGN
{
    enum class EventType
    {
        // Window
        //WINDOW_CLOSE,
        //WINDOW_RESIZE,
        //WINDOW_FOCUS,
        //WINDOW_LOST_FOCUS,
        //WINDOW_MOVED,

        // Key
        //KEY_PRESSED,
        //KEY_RELEASED,

        // Mouse
        //MOUSE_BUTTON_PRESSED,
        //MOUSE_BUTTON_RELEASED,
        //MOUSE_MOVED,
        //MOUSE_SCROLLED,

        DEBUG1,
        DEBUG2,
        DEBUG3,

        _COUNT
    };

    union EventData
    {
        void* Debug;
    };

    class EventManager;

    class EventListener
    {
    public:
        explicit EventListener(const List<EventType>& types, std::shared_ptr<EventManager> manager);
        virtual ~EventListener();

        EventListener(EventListener&&) noexcept;
        EventListener& operator=(EventListener&&) noexcept;

        EventListener(const EventListener&) = delete;
        EventListener& operator=(const EventListener&) = delete;

        virtual bool OnEvent(EventType type, EventData data) = 0;

    private:
        size_t m_Id;
        std::shared_ptr<EventManager> m_Manager;
    };

    class EventManager
    {
    public:
        EventManager();

        void TriggerEvent(EventType type, EventData data);

    private:
        size_t AddListener(EventListener* listener, const List<EventType>& types);
        void RemoveListener(size_t id);
        void MoveListener(size_t id, EventListener* listener);

        List<EventListener*> m_Listeners;
        HashMap<EventType, List<size_t>> m_ListenerMap;
        ssize_t m_NextFree = -1;

        friend EventListener;
    };
}
