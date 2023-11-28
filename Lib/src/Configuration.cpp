//
// Created by Killian on 28/11/2023.
//
#include "Configuration.h"

namespace NGN
{
    Configuration::Configuration(const List<String>& args)
    {
        enum class ParseState
        {
            None,
            LogLevel
        };

        auto state = ParseState::None;

        for (const auto& arg : args)
        {
            if (state == ParseState::None)
            {
                if (arg == "--log-level")
                {
                    state = ParseState::LogLevel;
                }
                else if(arg.StartsWith("-l="))
                {
                    this->log_level = LogLevel::FromConfigString(arg.Split("=")[1]);
                }
            }
            else if (state == ParseState::LogLevel)
            {
                this->log_level = LogLevel::FromConfigString(arg);
                state = ParseState::None;
            }
        }
    }
}