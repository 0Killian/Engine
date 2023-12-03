//
// Created by Killian on 28/11/2023.
//
#pragma once

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

        List(std::initializer_list<T> list)
            : m_Data(list)
        {}

        void PushBack(const T& value)
        {
            m_Data.push_back(value);
        }

        void Erase(size_t index)
        {
            m_Data.erase(m_Data.begin() + index);
        }

        void Reserve(size_t size)
        {
            m_Data.reserve(size);
        }

        void Clear()
        {
            m_Data.clear();
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
}