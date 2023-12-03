//
// Created by Killian on 30/11/2023.
//
#include "Window.h"

#include <Events.h>
#include <optional>

#include <utility>

#include "D3D11/Renderer.h"

#include "win.h"

namespace Windows
{
    std::optional<NGN::Key> TranslateKey(const WORD wp, const bool lastRCtrlState, const bool lastRShiftState, const bool lastRAltState, const bool pressed)
    {
        switch (wp)
        {
        case 'A': return NGN::Key::A;
        case 'B': return NGN::Key::B;
        case 'C': return NGN::Key::C;
        case 'D': return NGN::Key::D;
        case 'E': return NGN::Key::E;
        case 'F': return NGN::Key::F;
        case 'G': return NGN::Key::G;
        case 'H': return NGN::Key::H;
        case 'I': return NGN::Key::I;
        case 'J': return NGN::Key::J;
        case 'K': return NGN::Key::K;
        case 'L': return NGN::Key::L;
        case 'M': return NGN::Key::M;
        case 'N': return NGN::Key::N;
        case 'O': return NGN::Key::O;
        case 'P': return NGN::Key::P;
        case 'Q': return NGN::Key::Q;
        case 'R': return NGN::Key::R;
        case 'S': return NGN::Key::S;
        case 'T': return NGN::Key::T;
        case 'U': return NGN::Key::U;
        case 'V': return NGN::Key::V;
        case 'W': return NGN::Key::W;
        case 'X': return NGN::Key::X;
        case 'Y': return NGN::Key::Y;
        case 'Z': return NGN::Key::Z;
        case '0': return NGN::Key::NUM0;
        case '1': return NGN::Key::NUM1;
        case '2': return NGN::Key::NUM2;
        case '3': return NGN::Key::NUM3;
        case '4': return NGN::Key::NUM4;
        case '5': return NGN::Key::NUM5;
        case '6': return NGN::Key::NUM6;
        case '7': return NGN::Key::NUM7;
        case '8': return NGN::Key::NUM8;
        case '9': return NGN::Key::NUM9;
        case VK_ESCAPE: return NGN::Key::ESCAPE;
        case VK_CONTROL:
            if((GetKeyState(VK_RCONTROL) & 0x8000 && !lastRCtrlState && pressed) || (!(GetKeyState(VK_RCONTROL) & 0x8000) && lastRCtrlState && !pressed))
                return NGN::Key::RCONTROL;
            return NGN::Key::LCONTROL;
        case VK_SHIFT:
            if((GetKeyState(VK_RSHIFT) & 0x8000 && !lastRShiftState && pressed) || (!(GetKeyState(VK_RSHIFT) & 0x8000) && lastRShiftState && !pressed))
                return NGN::Key::RSHIFT;
            return NGN::Key::LSHIFT;
        case VK_MENU:
            if((GetKeyState(VK_RMENU) & 0x8000 && !lastRAltState && pressed) || (!(GetKeyState(VK_RMENU) & 0x8000) && lastRAltState && !pressed))
                return NGN::Key::RALT;
            return NGN::Key::LALT;
        case VK_LWIN: return NGN::Key::LSYSTEM;
        case VK_RWIN: return NGN::Key::RSYSTEM;
        case VK_APPS: return NGN::Key::MENU;
        case VK_OEM_4: return NGN::Key::LBRACKET; // Left bracket key '['
        case VK_OEM_6: return NGN::Key::RBRACKET; // Right bracket key ']'
        case VK_OEM_1: return NGN::Key::SEMICOLON; // Semicolon key ';'
        case VK_OEM_COMMA: return NGN::Key::COMMA;
        case VK_OEM_PERIOD: return NGN::Key::PERIOD;
        case VK_OEM_7: return NGN::Key::QUOTE; // Quote key '''
        case VK_OEM_2: return NGN::Key::SLASH; // Slash key '/'
        case VK_OEM_5: return NGN::Key::BACKSLASH;
        case VK_OEM_3: return NGN::Key::TILDE; // Tilde key '~'
        case VK_OEM_PLUS: return NGN::Key::EQUAL;
        case VK_OEM_MINUS: return NGN::Key::DASH;
        case VK_SPACE: return NGN::Key::SPACE;
        case VK_RETURN: return NGN::Key::RETURN;
        case VK_BACK: return NGN::Key::BACKSPACE;
        case VK_TAB: return NGN::Key::TAB;
        case VK_PRIOR: return NGN::Key::PAGEUP;
        case VK_NEXT: return NGN::Key::PAGEDOWN;
        case VK_END: return NGN::Key::END;
        case VK_HOME: return NGN::Key::HOME;
        case VK_INSERT: return NGN::Key::INSERT;
        case VK_DELETE: return NGN::Key::DELETE;
        case VK_ADD: return NGN::Key::ADD;
        case VK_SUBTRACT: return NGN::Key::SUBTRACT;
        case VK_MULTIPLY: return NGN::Key::MULTIPLY;
        case VK_DIVIDE: return NGN::Key::DIVIDE;
        case VK_LEFT: return NGN::Key::LEFT;
        case VK_RIGHT: return NGN::Key::RIGHT;
        case VK_UP: return NGN::Key::UP;
        case VK_DOWN: return NGN::Key::DOWN;
        case VK_NUMPAD0: return NGN::Key::NUMPAD0;
        case VK_NUMPAD1: return NGN::Key::NUMPAD1;
        case VK_NUMPAD2: return NGN::Key::NUMPAD2;
        case VK_NUMPAD3: return NGN::Key::NUMPAD3;
        case VK_NUMPAD4: return NGN::Key::NUMPAD4;
        case VK_NUMPAD5: return NGN::Key::NUMPAD5;
        case VK_NUMPAD6: return NGN::Key::NUMPAD6;
        case VK_NUMPAD7: return NGN::Key::NUMPAD7;
        case VK_NUMPAD8: return NGN::Key::NUMPAD8;
        case VK_NUMPAD9: return NGN::Key::NUMPAD9;
        case VK_F1: return NGN::Key::F1;
        case VK_F2: return NGN::Key::F2;
        case VK_F3: return NGN::Key::F3;
        case VK_F4: return NGN::Key::F4;
        case VK_F5: return NGN::Key::F5;
        case VK_F6: return NGN::Key::F6;
        case VK_F7: return NGN::Key::F7;
        case VK_F8: return NGN::Key::F8;
        case VK_F9: return NGN::Key::F9;
        case VK_F10: return NGN::Key::F10;
        case VK_F11: return NGN::Key::F11;
        case VK_F12: return NGN::Key::F12;
        case VK_F13: return NGN::Key::F13;
        case VK_F14: return NGN::Key::F14;
        case VK_F15: return NGN::Key::F15;
        case VK_PAUSE: return NGN::Key::PAUSE;
        default: return std::nullopt;
        }
    }
}

namespace NGN
{
    bool s_ClassRegistered = false;
    uint64_t s_nextId = 0;

    struct WindowInner
    {
        HWND m_Handle = nullptr;
        uint64_t m_Id = s_nextId++;
        std::shared_ptr<EventManager> m_EventManager;
        Logger& m_Logger;
        std::unique_ptr<Renderer> m_Renderer = nullptr;

        bool m_LastRCtrlState = false;
        bool m_LastRShiftState = false;
        bool m_LastRAltState = false;

        explicit WindowInner(Logger& logger)
            : m_Logger(logger)
        {
        }

        LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp)
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

            case WM_SIZE:
            {
                auto width = LOWORD(lp);
                auto height = HIWORD(lp);

                EventData data = {
                    .WindowSize = {
                        .ID =  m_Id,
                        .Width = width,
                        .Height = height
                    }
                };

                m_EventManager->TriggerEvent(EventType::WINDOW_RESIZE, data);

                return 0;
            }

            case WM_SETFOCUS:
            {
                EventData data = {
                    .WindowID =  m_Id,
                };

                m_EventManager->TriggerEvent(EventType::WINDOW_FOCUS, data);

                return 0;
            }

            case WM_KILLFOCUS:
            {
                EventData data = {
                    .WindowID =  m_Id,
                };

                m_EventManager->TriggerEvent(EventType::WINDOW_LOST_FOCUS, data);

                return 0;
            }

            case WM_MOVE:
            {
                auto x = LOWORD(lp);
                auto y = HIWORD(lp);

                EventData data = {
                    .WindowMoved = {
                        .ID =  m_Id,
                        .X = x,
                        .Y = y
                    }
                };

                m_EventManager->TriggerEvent(EventType::WINDOW_MOVED, data);

                return 0;
            }

            case WM_SYSKEYUP:
            case WM_SYSKEYDOWN:
            case WM_KEYDOWN:
            case WM_KEYUP:
            {
                auto win_key = static_cast<WORD>(wp);
                auto key = Windows::TranslateKey(win_key, m_LastRCtrlState, m_LastRShiftState, m_LastRAltState, message == WM_KEYDOWN || message == WM_SYSKEYDOWN);
                if(!key)
                {
                    m_Logger.Warning() << "Unknown key: " << win_key << Logger::EndLine;
                    return DefWindowProcA(hwnd, message, wp, lp);
                }

                switch(*key)
                {
                case Key::RCONTROL:
                    m_LastRCtrlState = message == WM_KEYDOWN || message == WM_SYSKEYDOWN;
                    break;

                case Key::RSHIFT:
                    m_LastRShiftState = message == WM_KEYDOWN || message == WM_SYSKEYDOWN;
                    break;

                case Key::RALT:
                    m_LastRAltState = message == WM_KEYDOWN || message == WM_SYSKEYDOWN;
                    break;

                default: break;
                }

                EventData data = {
                    .Key = *key
                };

                if(message == WM_KEYDOWN || message == WM_SYSKEYDOWN)
                {
                    m_EventManager->TriggerEvent(EventType::KEY_PRESSED, data);
                }
                else
                {
                    m_EventManager->TriggerEvent(EventType::KEY_RELEASED, data);
                }

                return 0;
            }

            case WM_LBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_XBUTTONDOWN:
            {
                EventData data{};
                if(message == WM_LBUTTONDOWN)
                    data.MouseButton = MouseButton::LEFT;
                else if(message == WM_RBUTTONDOWN)
                    data.MouseButton = MouseButton::RIGHT;
                else if(message == WM_MBUTTONDOWN)
                    data.MouseButton = MouseButton::MIDDLE;
                else if(message == WM_XBUTTONDOWN)
                    //data.MouseButton = MouseButton::EXTRA;
                    return DefWindowProcA(hwnd, message, wp, lp);

                m_EventManager->TriggerEvent(EventType::MOUSE_BUTTON_PRESSED, data);

                return 0;
            }

            case WM_LBUTTONUP:
            case WM_RBUTTONUP:
            case WM_MBUTTONUP:
            case WM_XBUTTONUP:
            {
                EventData data{};
                if(message == WM_LBUTTONUP)
                    data.MouseButton = MouseButton::LEFT;
                else if(message == WM_RBUTTONUP)
                    data.MouseButton = MouseButton::RIGHT;
                else if(message == WM_MBUTTONUP)
                    data.MouseButton = MouseButton::MIDDLE;
                else if(message == WM_XBUTTONUP)
                    //data.MouseButton = MouseButton::EXTRA;
                        return DefWindowProcA(hwnd, message, wp, lp);

                m_EventManager->TriggerEvent(EventType::MOUSE_BUTTON_RELEASED, data);

                return 0;
            }

            case WM_MOUSEMOVE:
            {
                auto x = LOWORD(lp);
                auto y = HIWORD(lp);

                EventData data = {
                    .MouseMoved = {
                        .X = static_cast<float>(x),
                        .Y = static_cast<float>(y)
                    }
                };

                m_EventManager->TriggerEvent(EventType::MOUSE_MOVED, data);

                return 0;
            }

            case WM_MOUSEWHEEL:
            {
                auto x = LOWORD(wp);
                auto y = HIWORD(wp);

                EventData data = {
                    .MouseScrolled = {
                        .X = static_cast<float>(x),
                        .Y = static_cast<float>(y)
                    }
                };

                m_EventManager->TriggerEvent(EventType::MOUSE_SCROLLED, data);

                return 0;
            }

            default:
                return DefWindowProcA(hwnd, message, wp, lp);
            }
        }

        // ReSharper disable once CppParameterMayBeConst
        static LRESULT CALLBACK WndProcStub(HWND hwnd, const UINT message, const WPARAM wp, const LPARAM lp)
        {
            const auto window = reinterpret_cast<WindowInner*>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));
            return window->WndProc(hwnd, message, wp, lp);
        }
    };

    // ReSharper disable once CppParameterMayBeConst
    LRESULT CALLBACK WndProcSetup(HWND hwnd, const UINT message, const WPARAM wp, const LPARAM lp)
    {
        if(message == WM_NCCREATE)
        {
            const auto cs = reinterpret_cast<CREATESTRUCT*>(lp);
            auto window = static_cast<WindowInner*>(cs->lpCreateParams);
            SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
            SetWindowLongPtrA(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowInner::WndProcStub));
            return window->WndProc(hwnd, message, wp, lp);
        }

        return DefWindowProcA(hwnd, message, wp, lp);
    }

    void RegisterClass()
    {
        const auto hInstance = GetModuleHandleA(nullptr);
        const auto className = "NGNWindowClass";

        const auto wndClass = WNDCLASSEXA {
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

    Window::Window(const Specification& spec, const std::shared_ptr<EventManager>& manager, Logger& logger, Configuration& config)
    {
        if(!s_ClassRegistered)
        {
            RegisterClass();
        }

        const auto hInstance = GetModuleHandleA(nullptr);
        const auto className = "NGNWindowClass";

        m_Inner = new WindowInner(logger);
        static_cast<WindowInner*>(m_Inner)->m_EventManager = manager;

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

        switch(spec.API)
        {
        case RenderAPI::NONE:
            break;

        case RenderAPI::D3D11:
            static_cast<WindowInner*>(m_Inner)->m_Renderer = std::make_unique<D3D11::Renderer>(hwnd, logger, manager, config, spec.Width, spec.Height, static_cast<WindowInner*>(m_Inner)->m_Id);
        }
    }

    uint64_t Window::GetID() const
    {
        return static_cast<WindowInner*>(m_Inner)->m_Id;
    }

    Window::~Window()
    {
        if(m_Inner != nullptr)
        {
            DestroyWindow(static_cast<WindowInner*>(m_Inner)->m_Handle);
            delete static_cast<WindowInner*>(m_Inner);
        }
    }

    void Window::PollEvents() const
    {
        MSG msg;
        while(PeekMessageA(&msg, static_cast<WindowInner*>(m_Inner)->m_Handle, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
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

    Renderer& Window::GetRenderer() const
    {
        return *static_cast<WindowInner*>(m_Inner)->m_Renderer;
    }
}