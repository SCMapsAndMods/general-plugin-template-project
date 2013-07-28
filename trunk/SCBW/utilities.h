// Contains various utility functions for use in injections.h.

#pragma once
#include "scbwdata.h"

namespace scbw {
//Kills the unit at the given position in the unitTable.
//Usage:	killUnit(0);
inline void killUnit(int unitIndex) {
  if (unitIndex < 0 || unitIndex > UNIT_ARRAY_LENGTH) return;
  CUnit* unit = &unitTable[unitIndex];
  unit->mainOrderId = 0;
}

//Kills any unit with 0 health (a common problem when you manually subtract unit HP).
//Usage:	killUnitsWithZeroHp();
inline void killUnitsWithZeroHp() {
  CUnit* unit;
  for(int i = 0; i < UNIT_ARRAY_LENGTH; ++i) {
    unit = &unitTable[i];
    if (unit->hitPoints == 0)
      unit->mainOrderId = 0;
  }
}

//Sets the Anywhere location.
inline void setAnywhereLocation() {
  LOCATION* location;
  location = &locationTable[63];
  location->topLeftX = 0;
  location->topLeftY = 0;
  location->bottomRightX = map_dimensions->width * 32;
  location->bottomRightY = map_dimensions->height * 32;
  location->flags = 63;
}

inline void setLocation(int locNumber, int topLeftX, int topLeftY, int bottomRightX, int bottomRightY, int flags) {
  LOCATION* location;
  location = &locationTable[locNumber];
  location->topLeftX = topLeftX;
  location->topLeftY = topLeftY;
  location->bottomRightX = bottomRightX;
  location->bottomRightY = bottomRightY;
  location->flags = flags;
}

// Functionally identical to the [playfram] opcode in iscript.bin.
inline void playFrame(CImage* image, u16 frameset) {
  if (image->frameSet != frameset) {
    image->frameSet = frameset;
    u16 frameIndex = frameset + image->direction;
    if (image->frameIndex != frameIndex) {
      image->flags |= 0x1;	//Order the game to redraw the image
      image->frameIndex = frameIndex;
    }
  }
}

}
