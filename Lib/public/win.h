//
// Created by Killian on 03/12/2023.
//
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef RegisterClass
#undef DELETE
#undef MOUSE_MOVED
#undef near
#undef far
#undef min
#undef max
#undef DEFINE_GUID
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	EXTERN_C const GUID name