//Contains definitions of basic data types.

#pragma once

//Include <algorithm> to use this
#define CLAMP(x, minVal, maxVal) (std::min(std::max((x), (minVal)), (maxVal)))

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#ifdef _MSC_VER
typedef unsigned __int32  u32;
typedef __int32           s32;
typedef unsigned __int16  u16;
typedef __int16           s16;
typedef unsigned __int8   u8;
typedef __int8            s8;
typedef u8                UNK;
typedef int               Bool32;
typedef __int8            Bool8;
#endif
