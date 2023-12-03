//
// Created by Killian on 28/11/2023.
//
#pragma once
#include "Containers/String.h"

namespace NGN
{
    template <typename T>
    String ToString(T value) = delete;

    String ToHex(uint32_t value);
}
