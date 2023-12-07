//
// Created by Killian on 28/11/2023.
//
#pragma once

#include "Containers/String.h"
#include "Logger.h"

namespace NGN
{
    template <typename T>
    concept ConfigValue = requires(String str, T t)
    {
        { T::FromConfigString(str) } -> std::convertible_to<T>;
        { t.ToConfigString() } -> std::convertible_to<String>;
    };

    class Configuration
    {
    public:
        explicit Configuration(const List<String>& args);

        static_assert(ConfigValue<LogLevel>);

        LogLevel LogLevel = LogLevel::Info;
        bool VSync = false;
        String BaseAssetFolder = "./Assets";
    };

    inline Logger::LogChannel& operator<<(Logger::LogChannel& channel, const Configuration& config)
    {
        channel << "Configuration: { LogLevel: " << config.LogLevel.ToConfigString() << ", VSync: " << config.VSync << " }";
        return channel;
    }
}