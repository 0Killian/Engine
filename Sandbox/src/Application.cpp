//
// Created by Killian on 27/11/2023.
//

#include <optional>
#include <Window.h>

#include <utility>

#include "Engine.h"
#include "Renderer.h"

#include <fstream>
#include <Components.h>

class Sandbox final : public NGN::Application
{
public:
    explicit Sandbox(const NGN::List<NGN::String>& args)
        : Application(args)
    {}

    void OnUpdate() override
    {}

protected:
	void InitInner() override
	{}
};

NGN_ENTRY_POINT(Sandbox)