//
// Created by Killian on 27/11/2023.
//

#include "Engine.h"

class Sandbox final : public NGN::Application
{
public:
    Sandbox()
    {
        std::cout << "Sandbox started" << std::endl;
    }
};

NGN_ENTRY_POINT(Sandbox)