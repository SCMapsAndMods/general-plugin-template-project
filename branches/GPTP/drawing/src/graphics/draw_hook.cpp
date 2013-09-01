#include "draw_hook.h"
#include "../SCBW/scbwdata.h"
#include "../tools.h"
#include "graphics_misc.h"

//-------- Draw hook taken from BWAPI --------//
static bool wantRefresh = false;
static void __stdcall DrawHook(graphics::Bitmap *surface, Bounds *bounds)
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
  if (graphics::drawAllShapes() > 0)
    wantRefresh = true;
}

void InjectDrawHook() {
  memoryPatch4((void*)0x004BD68D, (u32)DrawHook);
}
