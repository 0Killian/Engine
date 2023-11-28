//
// Created by Killian on 27/11/2023.
//

#include "Engine.h"

class Sandbox final : public NGN::Application
{
public:
    explicit Sandbox(const NGN::List<NGN::String>& args)
        : Application(args)
    {
        m_Logger.Info() << "Sandbox Created" << NGN::Logger::EndLine;
    }
};

NGN_ENTRY_POINT(Sandbox)