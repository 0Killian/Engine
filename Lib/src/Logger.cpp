//
// Created by Killian on 28/11/2023.
//
#include "../public/Logger.h"

#include "Containers/String.h"
#include <ostream>
#include <chrono>
#include <iostream>
#include <ctime>

namespace NGN
{
    LogLevel LogLevel::FromConfigString(const String& str)
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

    [[nodiscard]] String LogLevel::ToConfigString() const
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

    void Logger::LogChannel::LogPrefix()
    {
        if(m_newLine)
        {
            const auto now = std::chrono::system_clock::now();
            const auto now_c = std::chrono::system_clock::to_time_t(now);
            const auto tm = std::localtime(&now_c);

            std::cout << "[" << std::put_time(tm, "%Y/%m/%d %H:%M:%S") <<  "] " << m_ChannelName << ": ";
        }


        m_newLine = false;
    }


    Logger::LogChannel& Logger::LogChannel::operator<<(const char* str)
    {
        LogPrefix();

        std::cout << str;

        return *this;
    }

    Logger::LogChannel& Logger::LogChannel::operator<<(int32_t i)
    {
        LogPrefix();

        std::cout << i;

        return *this;
    }

    Logger::LogChannel& Logger::LogChannel::operator<<(uint32_t i)
    {
        LogPrefix();

        std::cout << i;

        return *this;
    }

    Logger::LogChannel& Logger::LogChannel::operator<<(int64_t i)
    {
        LogPrefix();

        std::cout << i;

        return *this;
    }

    Logger::LogChannel& Logger::LogChannel::operator<<(uint64_t i)
    {
        LogPrefix();

        std::cout << i;

        return *this;
    }

    Logger::LogChannel& Logger::LogChannel::operator<<(float f)
    {
        LogPrefix();

        std::cout << f;

        return *this;
    }

    Logger::LogChannel& Logger::LogChannel::operator<<(double d)
    {
        LogPrefix();

        std::cout << d;

        return *this;
    }

    Logger::LogChannel& Logger::LogChannel::operator<<(bool b)
    {
        LogPrefix();

        std::cout << b;

        return *this;
    }

    Logger::LogChannel& Logger::LogChannel::operator<<(Marker marker)
    {
        switch (marker)
        {
        case EndLine:
            std::cout << std::endl;
            m_newLine = true;
            break;
        }

        return *this;
    }

    Logger::Logger(LogLevel maxLevel)
    {
        switch(static_cast<LogLevel::Value>(maxLevel))
        {
        case LogLevel::Trace:
            this->m_Trace = std::make_unique<LogChannel>("TRACE");

        case LogLevel::Debug:
            this->m_Debug = std::make_unique<LogChannel>("DEBUG");

        case LogLevel::Info:
            this->m_Info = std::make_unique<LogChannel>("INFO");

        case LogLevel::Warning:
            this->m_Warning = std::make_unique<LogChannel>("WARNING");

        case LogLevel::Error:
            this->m_Error = std::make_unique<LogChannel>("ERROR");

        case LogLevel::Critical:
            this->m_Critical = std::make_unique<LogChannel>("CRITICAL");
            break;

        default: break;
        }
    }

}
