//
// Created by Killian on 28/11/2023.
//
#pragma once

#include "Engine.h"
#include "Platform.h"
#include "Configuration.h"
#include "Events.h"
#include "Input.h"
#include "Logger.h"
#include "Containers/String.h"
#include "ResourceManager.h"
#include "Window.h"
#include "Renderer.h"
#include "entt.h"
#include "GUI.h"

namespace NGN
{
    class Application
    {
    public:
        explicit Application(const List<String>& args);
        virtual ~Application();

        void Init();
        void Run();

        virtual void OnUpdate() = 0;

        void Exit();

        template <typename T>
        static void Create(const List<String>& args) { s_Instance = new T(args); }
        static Application& Get() { return *s_Instance; }
        static void Destroy() { delete s_Instance; }

        Configuration& GetConfiguration() { return m_Configuration; }
		Logger& GetLogger() { return m_Logger; }
		EventManager& GetEventManager() { return *m_EventManager; }
		Platform& GetPlatform() { return m_Platform; }
		Input& GetInput() { return *m_Input; }
        ResourceManager& GetResourceManager() { return m_ResourceManager; }
        Window& GetWindow() { return m_Window; }
        Renderer& GetRenderer() { return m_Window.GetRenderer(); }
        entt::registry& GetRegistry() { return m_Registry; }
        GUI& GetGUI() { return m_GUI; }

    private:
        void* m_Inner = nullptr;

    protected:
        Configuration m_Configuration;
        Logger m_Logger;
        EventManager* m_EventManager;
        Platform m_Platform;
        std::unique_ptr<Input> m_Input;
        ResourceManager m_ResourceManager;
        GUI m_GUI;
        Window m_Window;
        entt::registry m_Registry;
        bool m_ShouldExit = false;

        static Application* s_Instance;

        virtual void InitInner() = 0;
    };
}
