#pragma once
#include "../SCBW/scbwdata.h"

namespace offsets {
  const u32 InjectScreenUpdateAddr  = 0x004BD68D;
}

void customDrawing();

//----------------------------------------------- DRAW HOOK --------------------------------------------------
static bool wantRefresh = false;
static void __stdcall DrawHook(Bitmap *surface, Bounds *bounds)
{
  if (wantRefresh) {
    wantRefresh = false;
    memset(refreshRegions, 1, 1200);
  }

  oldDrawGameProc(surface, bounds);

  customDrawing();

  //if ( BW::BWDATA::GameScreenBuffer->isValid() )
  //{
  //  unsigned int numShapes = BWAPI::BroodwarImpl.drawShapes();
  //  
  //  if ( numShapes )
  //    wantRefresh = true;
  //}
}