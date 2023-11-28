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

            LogChannel& operator<<(const char* str) final { return *this; }
            LogChannel& operator<<(int32_t i) final { return *this; }
            LogChannel& operator<<(uint32_t i) final { return *this; }
            LogChannel& operator<<(int64_t i) final { return *this; }
            LogChannel& operator<<(uint64_t i) final { return *this; }
            LogChannel& operator<<(float f) final { return *this; }
            LogChannel& operator<<(double d) final { return *this; }
            LogChannel& operator<<(bool b) final { return *this; }
            LogChannel& operator<<(Marker marker) final { return *this; }
        };

        explicit Logger(LogLevel maxLevel);

        [[nodiscard]] LogChannel& Trace() const { return *m_Trace; }
        [[nodiscard]] LogChannel& Debug() const { return *m_Debug; }
        [[nodiscard]] LogChannel& Info() const { return *m_Info; }
        [[nodiscard]] LogChannel& Warning() const { return *m_Warning; }
        [[nodiscard]] LogChannel& Error() const { return *m_Error; }
        [[nodiscard]] LogChannel& Critical() const { return *m_Critical; }

    private:
        std::unique_ptr<LogChannel> m_Trace = std::make_unique<DisabledLogChannel>("");
        std::unique_ptr<LogChannel> m_Debug = std::make_unique<DisabledLogChannel>("");
        std::unique_ptr<LogChannel> m_Info = std::make_unique<DisabledLogChannel>("");
        std::unique_ptr<LogChannel> m_Warning = std::make_unique<DisabledLogChannel>("");
        std::unique_ptr<LogChannel> m_Error = std::make_unique<DisabledLogChannel>("");
        std::unique_ptr<LogChannel> m_Critical = std::make_unique<DisabledLogChannel>("");
    };
}
