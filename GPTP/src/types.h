//Contains definitions of basic data types.

#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

//Based on http://msdn.microsoft.com/en-us/library/windows/desktop/ms679289.aspx
#define C_ASSERT(e) typedef char __C_ASSERT__[(e)?1:-1]

#ifdef _MSC_VER
typedef unsigned __int32 u32;
typedef __int32 s32;
typedef unsigned __int16 u16;
typedef __int16 s16;
typedef unsigned __int8 u8;
typedef __int8 s8;
typedef u8 UNK;
typedef int Bool32;
#endif
