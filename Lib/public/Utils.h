//
// Created by Killian on 28/11/2023.
//
#pragma once

namespace NGN
{
    template <typename T>
    concept ostreamable = requires(std::ostream& os, const T& t)
    {
        os << t;
    };
}