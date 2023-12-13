//
// Created by Killian on 28/11/2023.
//
#pragma once

#include <cstdint>
#include <memory>

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

        static LogLevel FromConfigString(const class String& str);
        [[nodiscard]] String ToConfigString() const;

    private:
        Value m_Value;
    };

    class Logger
    {
    public:
        enum Marker : uint8_t
        {
            EndLine
        };

        class LogChannel
        {
        public:
            explicit LogChannel(const char* name) : m_ChannelName(name) {}
            virtual ~LogChannel() = default;

            virtual LogChannel& operator<<(const char* str);
            virtual LogChannel& operator<<(int32_t i);
            virtual LogChannel& operator<<(uint32_t i);
            virtual LogChannel& operator<<(int64_t i);
            virtual LogChannel& operator<<(uint64_t i);
            virtual LogChannel& operator<<(float f);
            virtual LogChannel& operator<<(double d);
            virtual LogChannel& operator<<(bool b);
            virtual LogChannel& operator<<(Marker marker);

        private:
            void LogPrefix();

            const char* m_ChannelName;
            bool m_newLine = true;
        };

        class DisabledLogChannel final : public LogChannel
        {
        public:
            explicit DisabledLogChannel(const char* name) : LogChannel(name) {}

            LogChannel& operator<<(const char*) override { return *this; }
            LogChannel& operator<<(int32_t) override { return *this; }
            LogChannel& operator<<(uint32_t) override { return *this; }
            LogChannel& operator<<(int64_t) override { return *this; }
            LogChannel& operator<<(uint64_t) override { return *this; }
            LogChannel& operator<<(float) override { return *this; }
            LogChannel& operator<<(double) override { return *this; }
            LogChannel& operator<<(bool) override { return *this; }
            LogChannel& operator<<(Marker) override { return *this; }
        };

        explicit Logger(LogLevel maxLevel);
        ~Logger();

        [[nodiscard]] static LogChannel& Trace() { return *Get().m_Trace; }
        [[nodiscard]] static LogChannel& Debug() { return *Get().m_Debug; }
        [[nodiscard]] static LogChannel& Info() { return *Get().m_Info; }
        [[nodiscard]] static LogChannel& Warning() { return *Get().m_Warning; }
        [[nodiscard]] static LogChannel& Error() { return *Get().m_Error; }
        [[nodiscard]] static LogChannel& Critical() { return *Get().m_Critical; }

        static Logger& Get();

    private:
        LogChannel* m_Trace = new DisabledLogChannel("");
        LogChannel* m_Debug = new DisabledLogChannel("");
        LogChannel* m_Info = new DisabledLogChannel("");
        LogChannel* m_Warning = new DisabledLogChannel("");
        LogChannel* m_Error = new DisabledLogChannel("");
        LogChannel* m_Critical = new DisabledLogChannel("");
    };
}
