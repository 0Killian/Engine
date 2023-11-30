//
// Created by Killian on 30/11/2023.
//
#include "Window.h"

#include <Events.h>

#include <utility>

namespace Windows
{
#include <Windows.h>
#undef RegisterClass
}

namespace NGN
{
    bool s_ClassRegistered = false;
    uint64_t s_nextId = 0;

    struct WindowInner
    {
        Windows::HWND m_Handle;
        uint64_t m_Id = s_nextId++;
        std::shared_ptr<EventManager> m_EventManager;

        Windows::LRESULT CALLBACK WndProc(Windows::HWND hwnd, Windows::UINT message, Windows::WPARAM wp, Windows::LPARAM lp)
        {
            switch(message)
            {
            case WM_CLOSE:
            {
                EventData data = {
                    .WindowID =  m_Id,
                };

                m_EventManager->TriggerEvent(EventType::WINDOW_CLOSE, data);

                return 0;
            }

            default:
                return DefWindowProcA(hwnd, message, wp, lp);
            }
        }

        static Windows::LRESULT CALLBACK WndProcStub(Windows::HWND hwnd, Windows::UINT message, Windows::WPARAM wp, Windows::LPARAM lp)
        {
            auto window = reinterpret_cast<WindowInner*>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));
            return window->WndProc(hwnd, message, wp, lp);
        }
    };

    Windows::LRESULT CALLBACK WndProcSetup(Windows::HWND hwnd, Windows::UINT message, Windows::WPARAM wp, Windows::LPARAM lp)
    {
        if(message == WM_NCCREATE)
        {
            auto cs = reinterpret_cast<Windows::CREATESTRUCT*>(lp);
            auto window = static_cast<WindowInner*>(cs->lpCreateParams);
            SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<Windows::LONG_PTR>(window));
            SetWindowLongPtrA(hwnd, GWLP_WNDPROC, reinterpret_cast<Windows::LONG_PTR>(WindowInner::WndProcStub));
            return window->WndProc(hwnd, message, wp, lp);
        }

        return DefWindowProcA(hwnd, message, wp, lp);
    }

    void RegisterClass()
    {
        auto hInstance = Windows::GetModuleHandleA(nullptr);
        auto className = "NGNWindowClass";

        auto wndClass = Windows::WNDCLASSEXA {
            .cbSize = sizeof(Windows::WNDCLASSEXA),
            .style = CS_OWNDC,
            .lpfnWndProc = WndProcSetup,
            .cbClsExtra = 0,
            .cbWndExtra = 0,
            .hInstance = hInstance,
            .hIcon = nullptr,
            .hCursor = nullptr,
            .hbrBackground = nullptr,
            .lpszMenuName = nullptr,
            .lpszClassName = className,
            .hIconSm = nullptr
        };

        if(RegisterClassExA(&wndClass) == 0)
        {
            throw std::runtime_error("Failed to register window class");
        }

        s_ClassRegistered = true;
    }

    Window::Window(Specification spec, std::shared_ptr<EventManager> manager)
    {
        if(!s_ClassRegistered)
        {
            RegisterClass();
        }

        auto hInstance = Windows::GetModuleHandleA(nullptr);
        auto className = "NGNWindowClass";

        m_Inner = new WindowInner();
        static_cast<WindowInner*>(m_Inner)->m_EventManager = std::move(manager);

        auto hwnd = Windows::CreateWindowExA(
            0,
            className,
            spec.Title.GetData(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            static_cast<int32_t>(spec.Width),
            static_cast<int32_t>(spec.Height),
            nullptr,
            nullptr,
            hInstance,
            m_Inner
        );

        if(hwnd == nullptr)
        {
            throw std::runtime_error("Failed to create window");
        }

        Windows::ShowWindow(hwnd, SW_SHOW);

        static_cast<WindowInner*>(m_Inner)->m_Handle = hwnd;
    }

    uint64_t Window::GetID() const
    {
        return static_cast<WindowInner*>(m_Inner)->m_Id;
    }

    Window::~Window()
    {
        if(m_Inner != nullptr)
        {
            Windows::DestroyWindow(static_cast<WindowInner*>(m_Inner)->m_Handle);
            delete static_cast<WindowInner*>(m_Inner);
        }
    }

    void Window::PollEvents()
    {
        Windows::MSG msg;
        while(PeekMessageA(&msg, static_cast<WindowInner*>(m_Inner)->m_Handle, 0, 0, PM_REMOVE))
        {
            Windows::TranslateMessage(&msg);
            Windows::DispatchMessageA(&msg);
        }
    }

    Window::Window(Window&& other) noexcept
        : m_Inner(other.m_Inner)
    {
        other.m_Inner = nullptr;
    }

    Window& Window::operator=(Window&& other) noexcept
    {
        if(this != &other)
        {
            m_Inner = other.m_Inner;
            other.m_Inner = nullptr;
        }

        return *this;
    }
}