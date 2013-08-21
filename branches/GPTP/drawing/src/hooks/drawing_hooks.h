#pragma once
#include "../SCBW/scbwdata.h"
#include <string>

namespace offsets {
  const u32 InjectScreenUpdateAddr  = 0x004BD68D;
}

void addString(const std::string& str);
void flushString();
int customDrawing();

//----------------------------------------------- DRAW HOOK --------------------------------------------------
static bool wantRefresh = false;
static void __stdcall DrawHook(Bitmap *surface, Bounds *bounds)
{
  if (wantRefresh) {
    wantRefresh = false;
    memset(refreshRegions, 1, 1200);
  }

  oldDrawGameProc(surface, bounds);

  //if ( BW::BWDATA::GameScreenBuffer->isValid() )
  //{
  //  unsigned int numShapes = BWAPI::BroodwarImpl.drawShapes();
  //  
  //  if ( numShapes )
  //    wantRefresh = true;
  //}
  if (customDrawing() > 0)
    wantRefresh = true;
}