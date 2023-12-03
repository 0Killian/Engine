//
// Created by Killian on 03/12/2023.
//
#include "Utils.h"

namespace NGN
{
    String ToHex(const uint32_t value)
    {
        char buffer[11];
        sprintf_s(buffer, "0x%08X", value);
        return buffer;
    }
}