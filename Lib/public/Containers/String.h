//
// Created by Killian on 28/11/2023.
//
#pragma once

#include <string>

#include "List.h"
#include "Logger.h"

namespace NGN
{
    /* class String
    {
        // TODO: Implement own string class
    public:
        String()
        {
            m_Data = new char[1];
			m_Data[0] = '\0';
			m_Size = 0;
			m_Capacity = 1;
        }

        String(const char* string, size_t length = npos)
        {
            if(length == npos)
				length = strlen(string);

            m_Data = new char[length + 1];
            strcpy_s(m_Data, length + 1, string);
            m_Data[length] = '\0';

            m_Size = length;
            m_Capacity = m_Size + 1;
        }

        ~String()
        {
            if(m_Data != nullptr)
			    delete[] m_Data;
            m_Data = nullptr;
		}

        void Resize(size_t size)
        {
            if (size > m_Capacity)
            {
				char* newData = new char[size + 1];
                if(m_Size > 0)
				    strcpy_s(newData, m_Size, m_Data);
				delete[] m_Data;
				m_Data = newData;
				m_Capacity = size + 1;

                for(size_t i = m_Size; i < m_Capacity; i++)
					m_Data[i] = '\0';
			}

			m_Size = size;
        }

        size_t Find(const char* string, const size_t start = 0) const
        {
            for(size_t i = start; i < m_Size; i++)
			{
				if(m_Data[i] == string[0])
				{
					bool found = true;
					for(size_t j = 1; j < strlen(string); j++)
					{
						if(m_Data[i + j] != string[j])
						{
							found = false;
							break;
						}
					}

					if(found)
						return i;
				}
			}

            return npos;
        }

        List<String> Split(const char* delimiter) const
        {
            List<String> result;

            size_t last = 0;
            size_t next;

            while ((next = Find(delimiter, last)) != npos)
            {
                result.PushBack(SubStr(last, next - last));
                last = next + 1;
            }

            result.PushBack(SubStr(last));

            return result;
        }

        [[nodiscard]] String SubStr(const size_t start, const size_t length = std::string::npos) const
        {
            return String(m_Data + start, length);
        }

        [[nodiscard]] const char* GetData() const { return m_Data; }
        [[nodiscard]] char* GetData() { return m_Data; }

        bool operator==(const String& other) const
        {
            return strcmp(m_Data, other.m_Data) == 0;
        }

        bool operator==(const char* other) const
        {
            return strcmp(m_Data, other) == 0;
        }

        [[nodiscard]] bool StartsWith(const char* other) const
        {
            return Find(other) == 0;
        }

        [[nodiscard]] size_t Size() const { return m_Size; }

        String operator+(const String& other) const
        {
            String ret;
            ret.m_Data = new char[m_Size + other.m_Size + 1];
            strcpy_s(ret.m_Data, m_Size + 1, m_Data);
            strcpy_s(ret.m_Data + m_Size, other.m_Size + 1, other.m_Data);
            ret.m_Size = m_Size + other.m_Size;
            ret.m_Capacity = ret.m_Size + 1;

            return ret;
        }

        static constexpr size_t npos = -1;

    private:
        char* m_Data = nullptr;
        size_t m_Size;
        size_t m_Capacity;
    }; */

    class String
    {
        // TODO: Implement own string class
    public:
        String() = default;

        String(const char* string, size_t length = npos)
        {
            if(length == npos)
                length = strlen(string);
            m_Data = std::string(string, length);
        }

        void Resize(size_t size)
        {
            m_Data.resize(size);
        }

        size_t Find(const char* string, const size_t start = 0) const
        {
            return m_Data.find(string, start);
        }

        List<String> Split(const char* delimiter) const
        {
            List<String> result;

            size_t last = 0;
            size_t next;

            while ((next = Find(delimiter, last)) != npos)
            {
                result.PushBack(SubStr(last, next - last));
                last = next + 1;
            }

            result.PushBack(SubStr(last));

            return result;
        }

        [[nodiscard]] String SubStr(const size_t start, const size_t length = std::string::npos) const
        {
            String newString;
            newString.m_Data = m_Data.substr(start, length);
            return newString;
        }

        [[nodiscard]] const char* GetData() const { return m_Data.c_str(); }
        [[nodiscard]] char* GetData() { return m_Data.data(); }

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
            return Find(other) == 0;
        }

        [[nodiscard]] size_t Size() const { return m_Data.size(); }

        String operator+(const String& other) const
        {
            String newString;
            newString.m_Data = m_Data + other.m_Data;
            return newString;
        }

        static constexpr size_t npos = -1;

    private:
        std::string m_Data;
    };
    

    inline Logger::LogChannel& operator<<(Logger::LogChannel& channel, const String& string)
    {
        channel << string.GetData();
        return channel;
    }
}

template <>
struct std::equal_to<NGN::String>
{
    bool operator()(const NGN::String& lhs, const NGN::String& rhs) const
    {
        return lhs == rhs;
    }
};

template <>
struct std::hash<NGN::String>
{
    size_t operator()(const NGN::String& string) const
    {
        return std::hash<std::string>()(string.GetData());
    }
};
