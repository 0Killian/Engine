//
// Created by Killian on 28/11/2023.
//
#pragma once

#include "Utils.h"
#include <vector>

namespace NGN
{
    template <typename T>
    class List
    {
        // TODO: Implement own list class
    public:
        List() = default;

        List(size_t size, const T* data)
            : m_Data(data, data + size)
        {}

        void PushBack(const T& value)
        {
            m_Data.push_back(value);
        }

        [[nodiscard]] size_t Size() const { return m_Data.size(); }

        const T& operator[](size_t index) const { return m_Data[index]; }
        T& operator[](size_t index) { return m_Data[index]; }

        auto begin() const { return m_Data.cbegin(); }
        auto end() const { return m_Data.cend(); }

        auto begin() { return m_Data.begin(); }
        auto end() { return m_Data.end(); }

    private:
        std::vector<T> m_Data;
    };

    template <typename T>
    requires ostreamable<T>
    std::ostream& operator<<(std::ostream& os, const List<T>& list)
    {
        os << std::string("[");
        for (size_t i = 0; i < list.Size(); ++i)
        {
            os << list[i];
            if (i != list.Size() - 1)
                os << std::string(", ");
        }
        os << std::string("]");
        return os;
    }
}