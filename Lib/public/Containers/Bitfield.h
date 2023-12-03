//
// Created by Killian on 01/12/2023.
//
#pragma once

#include <bitset>

namespace NGN
{
    template <typename T>
    concept CountedEnum = requires(T t)
    {
        { static_cast<size_t>(T::_COUNT) } -> std::convertible_to<size_t>;
    };

    template <typename T>
    requires CountedEnum<T>
    class BitfieldProxy
    {
    public:
        BitfieldProxy(std::bitset<static_cast<size_t>(T::_COUNT)>& bits, const size_t index)
            : m_Bits(bits)
            , m_Index(index)
        {}

        BitfieldProxy& operator=(bool value)
        {
            m_Bits[m_Index] = value;
            return *this;
        }

        // ReSharper disable once CppNonExplicitConversionOperator
        operator bool() const // NOLINT(*-explicit-constructor)
        {
            return m_Bits[m_Index];
        }

    private:
        std::bitset<static_cast<size_t>(T::_COUNT)>& m_Bits;
        size_t m_Index;
    };

    template <typename T>
    requires CountedEnum<T>
    class Bitfield
    {
    public:
        Bitfield() = default;

        explicit Bitfield(std::true_type)
        {
            m_Bits.set();
        }

        BitfieldProxy<T> operator[](T type)
        {
            return BitfieldProxy<T>(m_Bits, static_cast<size_t>(type));
        }

    private:
        std::bitset<static_cast<size_t>(T::_COUNT)> m_Bits;
    };
}