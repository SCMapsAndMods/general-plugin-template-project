//Contains definitions of basic data types.

#pragma once

#include <cstdint>

//Include <algorithm> to use this
#define CLAMP(x, minVal, maxVal) (std::min(std::max((x), (minVal)), (maxVal)))

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

typedef std::uint32_t	u32;
typedef std::int32_t	s32;
typedef std::uint16_t	u16;
typedef std::int16_t	s16;
typedef std::uint8_t	u8;
typedef std::int8_t		s8;
typedef u8				UNK;
typedef s32				Bool32;
typedef s8				Bool8;