#include "graphics_errors.h"

namespace graphics {

int errorFlags;

void setError(GraphicsErrorId error) {
  errorFlags &= error;
}

void clearErrors() {
  errorFlags = 0;
}

void drawErrorMessages() {

#ifndef NDEBUG

  if (errorFlags & ERR_TOO_MANY_SHAPES) {
    //TODO: fill content
  }

  if (errorFlags & ERR_TOO_MANY_STRINGS) {
    //TODO: fill content
  }

  if (errorFlags & ERR_UKNOWN_SHAPE) {
  }

#endif

};

} //graphics
