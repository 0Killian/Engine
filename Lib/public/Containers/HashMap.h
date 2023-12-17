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
        [[nodiscard]] bool ContainsKey(const K& key) const { return m_Data.contains(key); }
        [[nodiscard]] const V& Get(const K& key) const { return m_Data.at(key); }
		[[nodiscard]] V& Get(const K& key) { return m_Data.at(key); }
        void Erase(const K& key) { m_Data.erase(key); }
        void Insert(const K& key, const V& value) { m_Data.insert({key, value}); }
        void Clear() { m_Data.clear(); }

        V& operator[](const K& key) { return m_Data[key]; }

        auto begin() const { return m_Data.cbegin(); }
        auto end() const { return m_Data.cend(); }
        auto begin() { return m_Data.begin(); }
        auto end() { return m_Data.end(); }

    private:
        std::unordered_map<K, V> m_Data;
    };
}