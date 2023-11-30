//
// Created by Killian on 30/11/2023.
//
#include "Window.h"

#include <Windows.h>

namespace Windows
{
#include <Windows.h>
#undef RegisterClass
}

namespace NGN
{
    bool s_ClassRegistered = false;

    struct WindowInner
    {
        HWND m_Handle;

        LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp)
        {
            switch(message)
            {
            default:
                return DefWindowProcA(hwnd, message, wp, lp);
            }
        }

        static LRESULT CALLBACK WndProcStub(HWND hwnd, UINT message, WPARAM wp, LPARAM lp)
        {
            auto window = reinterpret_cast<WindowInner*>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));
            return window->WndProc(hwnd, message, wp, lp);
        }
    };

    LRESULT CALLBACK WndProcSetup(HWND hwnd, UINT message, WPARAM wp, LPARAM lp)
    {
        if(message == WM_NCCREATE)
        {
            auto cs = reinterpret_cast<CREATESTRUCT*>(lp);
            auto window = reinterpret_cast<WindowInner*>(cs->lpCreateParams);
            SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
            SetWindowLongPtrA(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowInner::WndProcStub));
            return window->WndProc(hwnd, message, wp, lp);
        }

        return DefWindowProcA(hwnd, message, wp, lp);
    }

    void RegisterClass()
    {
        auto hInstance = GetModuleHandleA(nullptr);
        auto className = "NGNWindowClass";

        auto wndClass = WNDCLASSEXA {
            .cbSize = sizeof(WNDCLASSEXA),
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

    Window::Window(Specification spec)
    {
        if(!s_ClassRegistered)
        {
            RegisterClass();
        }

        auto hInstance = GetModuleHandleA(nullptr);
        auto className = "NGNWindowClass";

        m_Inner = new WindowInner();

        auto hwnd = CreateWindowExA(
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

        ShowWindow(hwnd, SW_SHOW);

        static_cast<WindowInner*>(m_Inner)->m_Handle = hwnd;
    }

    Window::~Window()
    {
        if(m_Inner != nullptr)
        {
            DestroyWindow(static_cast<WindowInner*>(m_Inner)->m_Handle);
            delete static_cast<WindowInner*>(m_Inner);
        }
    }

    void Window::PollEvents()
    {
        MSG msg;
        while(PeekMessageA(&msg, static_cast<WindowInner*>(m_Inner)->m_Handle, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
    }

}