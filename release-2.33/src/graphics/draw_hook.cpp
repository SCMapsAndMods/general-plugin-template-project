#include "draw_hook.h"
#include "../SCBW/api.h"
#include "../tools.h"
#include "graphics_misc.h"

//-------- Draw hook taken from BWAPI --------//
static bool wantRefresh = false;
static void __stdcall DrawHook(graphics::Bitmap *surface, Bounds *bounds)
{
  if (wantRefresh) {
    wantRefresh = false;
    scbw::refreshScreen();
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
  memoryPatch(0x004BD68D, &DrawHook);
}
