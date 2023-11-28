//
// Created by Killian on 27/11/2023.
//

#include "Engine.h"

class Editor final : public NGN::Application
{
public:
    Editor()
    {
        std::cout << "Editor started" << std::endl;
    }
};

NGN_ENTRY_POINT(Editor)