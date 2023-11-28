//
// Created by Killian on 28/11/2023.
//
#pragma once

#include <cstdint>

namespace NGN
{
    class LogLevel
    {
    public:
        enum Value : uint8_t
        {
            Trace,
            Debug,
            Info,
            Warning,
            Error,
            Critical
        };

        LogLevel() = default;
        constexpr LogLevel(const Value value) : m_Value(value) {}

        constexpr operator Value() const { return m_Value; }
        constexpr operator bool() const = delete;

        static constexpr LogLevel FromConfigString(const String& str)
        {
            if (str == "trace")
                return Trace;
            if (str == "debug")
                return Debug;
            if (str == "info")
                return Info;
            if (str == "warning")
                return Warning;
            if (str == "error" || str == "critical") // error is the minimum level
                return Error;

            return Info;
        }

        [[nodiscard]] constexpr String ToConfigString() const
        {
            switch (m_Value)
            {
                case Trace:
                    return String("trace");
                case Debug:
                    return String("debug");
                case Info:
                    return String("info");
                case Warning:
                    return String("warning");
                case Error:
                    return String("error");
                case Critical:
                    return String("critical");
            }

            return String("info");
        }

    private:
        Value m_Value;
    };
}
