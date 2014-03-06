#pragma once
#include "../../types.h"


namespace graphics { class Bitmap; }
struct bounds;

struct Layer {
  u8  buffers;
//u8  bits;
  u8  hasBeenRefreshed  : 1;
  u8  unk               : 7;
  u16 left;
  u16 top;
  u16 width;
  u16 height;
  u16 alignment;
  graphics::Bitmap  *pSurface;
  void (__stdcall *pUpdate)(graphics::Bitmap *pSurface, bounds *pBounds);
};

static_assert(sizeof(Layer) == 20, "The size of the Layer structure is invalid");


struct Layers {
  Layer mouse;
  Layer context;
  Layer dialog;
  Layer placeBuildingA;
  Layer placeBuildingB;
  Layer game;
};
