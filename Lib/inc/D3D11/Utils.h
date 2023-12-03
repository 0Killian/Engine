//
// Created by Killian on 03/12/2023.
//
#pragma once

#include "Utils.h"

#define THROW_IF_FAILED(expr) \
    { \
        HRESULT hr = (expr); \
        if(FAILED(hr)) \
        { \
            throw std::runtime_error((NGN::String("Failed to execute expression: " #expr ": ") + NGN::D3D11::GetErrorMessage(hr)).GetData()); \
        } \
    }

namespace NGN::D3D11
{
    inline String GetErrorMessage(const HRESULT hr)
    {
        char* msg;
        if(FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            hr,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<char*>(&msg),
            0,
            nullptr
        ) == 0)
        {
            return String("Unknown error: ") + ToHex(hr);
        }

        String result(msg);
        LocalFree(msg);
        return result;
    }
}
