//
// Created by Killian on 28/11/2023.
//
#pragma once

#include <Platform.h>

#include "Events.h"
#include "Logger.h"
#include "Containers/String.h"

namespace NGN
{
    class Application
    {
    public:
        explicit Application(const List<String>& args);
        virtual ~Application();

        void Run();

        virtual void OnUpdate() = 0;

        void Exit();

    private:
        void* m_Inner = nullptr;

    protected:
        Logger m_Logger;
        std::shared_ptr<EventManager> m_EventManager = std::make_shared<EventManager>(m_Logger);
        Platform m_Platform;
        bool m_ShouldExit = false;
    };
}
