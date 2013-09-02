//Simple error handling for drawing graphics

#pragma once

namespace graphics {

enum GraphicsErrorId {
  ERR_TOO_MANY_SHAPES   = 0x1,
  ERR_TOO_MANY_STRINGS  = 0x2,
  ERR_UKNOWN_SHAPE      = 0x4,
};

//Sets an error flag.
void setError(GraphicsErrorId error);

//Clears all error flags.
void clearErrors();

//Checks current error flags and draws appropriate error messages to the screen.
//If NDEBUG is defined (i.e. release builds), this function does nothing.
void drawErrorMessages();

} //graphics
