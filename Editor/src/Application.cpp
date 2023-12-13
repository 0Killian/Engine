//
// Created by Killian on 27/11/2023.
//

#include "Engine.h"

class Editor final : public NGN::Application
{
public:
    explicit Editor(const NGN::List<NGN::String>& args)
        : Application(args)
    {
    }

    void OnUpdate() override
    {
    }

    void InitInner() override
    {
	}
};

NGN_ENTRY_POINT(Editor)