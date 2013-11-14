#pragma once
#include "types.h"
#include "ini_comments.h"
#include <string>

namespace datcc {

template <typename T>
struct FlagNames {
  const char names[sizeof(T) * 8][50];

  std::string makeCommentString(T flag, size_t keyStrSize) const {
    char flagString[sizeof(T) * 8 + 1];
    bool isFirst = true;
    std::string str;

    T bit = 1;
    for (int i = 0; i < sizeof(T) * 8; ++i, bit <<= 1) {
      if (flag & bit) {
        flagString[sizeof(T) * 8 - 1 - i] = '1';
        if (!isFirst)
          str += " | ";

        str += names[i];
        isFirst = false;
      }
      else
        flagString[sizeof(T) * 8 - 1 - i] = '0';
    }

    flagString[sizeof(T) * 8] = '\0';
    return makeCommentPadding(flagString, keyStrSize) + str;
  }
};

extern const FlagNames<BYTE>  unitMovementFlags;
extern const FlagNames<DWORD> unitPrototypeFlags;
extern const FlagNames<BYTE>  unitGroupFlags;
extern const FlagNames<WORD>  unitAvailabilityFlags;
extern const FlagNames<WORD>  weaponTargetFlags;
extern const FlagNames<BYTE>  unknownFlags8;

} //datcc
