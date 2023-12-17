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

        List(const List& other)
			: m_Data(other.m_Data)
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

        void Resize(size_t size)
		{
			m_Data.resize(size);
		}

        void Insert(size_t index, const T& value)
		{
			m_Data.insert(m_Data.begin() + index, value);
		}

        [[nodiscard]] size_t Size() const { return m_Data.size(); }
        [[nodiscard]] T* GetData() { return m_Data.data(); }
        [[nodiscard]] const T* GetData() const { return m_Data.data(); }

        template <typename... Args>
        void EmplaceBack(Args&&... args)
		{
			m_Data.emplace_back(std::forward<Args>(args)...);
		}

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