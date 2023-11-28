//
// Created by Killian on 28/11/2023.
//
#pragma once

#include <string>

#include "List.h"

namespace NGN
{
    class String
    {
        // TODO: Implement own string class
    public:
        explicit String(const char* string)
            : m_Data(string)
        {}

        List<String> Split(const char* delimiter) const
        {
            List<String> result;

            size_t last = 0;
            size_t next;

            while ((next = m_Data.find(delimiter, last)) != std::string::npos)
            {
                result.PushBack(SubStr(last, next - last));
                last = next + 1;
            }

            result.PushBack(SubStr(last));

            return result;
        }

        [[nodiscard]] String SubStr(const size_t start, const size_t length = std::string::npos) const
        {
            return String(m_Data.substr(start, length).c_str());
        }

        [[nodiscard]] const char* GetData() const { return m_Data.c_str(); }

        bool operator==(const String& other) const
        {
            return m_Data == other.m_Data;
        }

        bool operator==(const char* other) const
        {
            return m_Data == other;
        }

        [[nodiscard]] bool StartsWith(const char* other) const
        {
            return m_Data.find(other) == 0;
        }

    private:
        std::string m_Data;
    };

    inline std::ostream& operator<<(std::ostream& os, const String& string)
    {
        os << std::string(string.GetData());
        return os;
    }
}
