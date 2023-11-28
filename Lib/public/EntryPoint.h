//
// Created by Killian on 27/11/2023.
//
#pragma once

#include <iostream>

#ifdef _WIN32
#include <Windows.h>

#define NGN_ENTRY_POINT(ApplicationClass) \
    int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) \
    { \
        try { \
            /* TODO: Parse arguments */ \
            ApplicationClass app = ApplicationClass(); \
            app.Run(); \
        } catch (const std::exception& e) { \
            std::cerr << e.what() << std::endl; \
        } \
    }
#else
#define NGN_ENTRY_POINT(ApplicationClass) \
    int main(int argc, char** argv) \
    { \
        try { \
            /* TODO: Parse arguments */ \
            ApplicationClass app = ApplicationClass(); \
            app.Run(); \
        } catch (const std::exception& e) { \
            std::cerr << e.what() << std::endl; \
        } \
    }
#endif