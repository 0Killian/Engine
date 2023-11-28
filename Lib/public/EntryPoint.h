//
// Created by Killian on 27/11/2023.
//
#pragma once

#include <iostream>
#include "Test.h"

#ifdef _WIN32
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char** argv)
#endif
{
    std::cout << "Hello World: " << Test() << std::endl;

    return 0;
}