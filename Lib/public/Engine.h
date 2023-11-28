//
// Created by Killian on 27/11/2023.
//
#pragma once

#ifdef NGN_EXPORT

#ifdef _WIN32
#define NGN_API __declspec(dllexport)
#else
#define NGN_API __attribute__((visibility("default")))
#endif

#else

#ifdef _WIN32
#define NGN_API __declspec(dllimport)
#else
#define NGN_API
#endif

#include "Test.h"
#include "EntryPoint.h"

#endif