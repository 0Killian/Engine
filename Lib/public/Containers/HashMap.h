//
// Created by Killian on 29/11/2023.
//
#pragma once

#include <unordered_map>

namespace NGN
{
    template <typename K, typename V>
    class HashMap
    {
        // TODO: Implement own HashMap class
    public:
        [[nodiscard]] size_t Size() const { return m_Data.size(); }

        const V& operator[](const K& key) const { return m_Data[key]; }
        V& operator[](const K& key) { return m_Data[key]; }

        auto begin() const { return m_Data.cbegin(); }
        auto end() const { return m_Data.cend(); }
        auto begin() { return m_Data.begin(); }
        auto end() { return m_Data.end(); }

    private:
        std::unordered_map<K, V> m_Data;
    };
}