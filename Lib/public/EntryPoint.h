//
// Created by Killian on 27/11/2023.
//
#pragma once

#include <iostream>

#include "Application.h"
#include "Containers/String.h"

#ifdef _WIN32
#include "win.h"

#define NGN_ENTRY_POINT(ApplicationClass) \
    int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) \
    {                                                           \
        try {                                                   \
            NGN::List<NGN::String> argv;                        \
            for (int i = 0; i < __argc; ++i)                    \
                argv.PushBack(NGN::String(__argv[i]));          \
                                                                \
            NGN::Application::Create<ApplicationClass>(argv);   \
            NGN::Application::Get().Init();                     \
            NGN::Application::Get().Run();                      \
        } catch (const std::exception& e) {                     \
            std::cerr << e.what() << std::endl;                 \
        }                                                       \
                                                                \
        return 0;                                               \
    }
#else
#define NGN_ENTRY_POINT(ApplicationClass) \
    int main(int argc, char** argv)                             \
    {                                                           \
        try {                                                   \
            NGN::List<NGN::String> _argv;                       \
            for (int i = 0; i < argc; ++i)                      \
                _argv.PushBack(NGN::String(argv[i]));           \
                                                                \
            NGN::Application::Create<ApplicationClass>(_argv);  \
            NGN::Application::Get().Init();					    \
            NGN::Application::Get().Run();                      \
        } catch (const std::exception& e) {                     \
            std::cerr << e.what() << std::endl;                 \
        }                                                       \
                                                                \
        return 0;                                               \
    }
#endif