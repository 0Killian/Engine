//
// Created by Killian on 28/11/2023.
//
#include "Configuration.h"
#include "Application.h"

namespace NGN
{
    Configuration& Configuration::Get()
	{
		return Application::Get().GetConfiguration();
	}

    Configuration::Configuration(const List<String>& args)
    {
        enum class ParseState
        {
            None,
            LogLevel,
            BaseAssetFolder
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
                    this->LogLevel = LogLevel::FromConfigString(arg.Split("=")[1]);
                }
                else if (arg == "--vsync")
                {
                    this->VSync = true;
                }
                else if (arg == "--assets")
                {
                    state = ParseState::BaseAssetFolder;
                }
                else if (arg.StartsWith("-A="))
                {
                    this->BaseAssetFolder = arg.Split("=")[1];
                }
            }
            else if (state == ParseState::LogLevel)
            {
                this->LogLevel = LogLevel::FromConfigString(arg);
                state = ParseState::None;
            }
            else if (state == ParseState::BaseAssetFolder)
            {
                this->BaseAssetFolder = arg;
                state = ParseState::None;
            }
        }
    }
}