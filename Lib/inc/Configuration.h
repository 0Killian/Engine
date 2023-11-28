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
        void ParseArgs(const List<String>& args);

        static_assert(ConfigValue<LogLevel>);

        LogLevel log_level = LogLevel::Info;
    };

    inline std::ostream& operator<<(std::ostream& os, const Configuration& config)
    {
        os << String("Configuration: { log_level: ") << config.log_level.ToConfigString() << String(" }");
        return os;
    }
}