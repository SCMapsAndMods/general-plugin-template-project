#pragma once
#include <cstdlib>

namespace datcc {

/// Condition: flags must be an integral type (long, short, char, etc).
template <typename FlagT>
const char* convertFlagsToStr(FlagT flags);

/// Converts a flag string to an unsigned long value.
unsigned long covertToFlags(const char *str);



//-------- Function definitions --------//

template <typename FlagT>
const char* convertFlagsToStr(FlagT flags) {
  static char buffer[100];
  int bitPos = sizeof(FlagT) * 8;
  buffer[bitPos--] = '\0';
  for (FlagT bit = 1; bitPos >= 0; --bitPos, bit <<= 1)
    buffer[bitPos] = (flags & bit ? '1' : '0');
  return buffer;
}

inline unsigned long convertToFlags(const char *str) {
  if (str == NULL) return 0;
  return strtoul(str, NULL, 2);
}

} //datcc
