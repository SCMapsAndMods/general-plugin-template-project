#pragma once
#include "../types.h"

namespace offsets {
  const u32 Hook_ScreenUpdateProc = 0x004BD627;
}

void customDrawing();

//Inject w/ jmpPatch
static void __declspec(naked) customDrawingWrapper() {
  __asm {
    PUSHAD
    MOV EBP, ESP
  }

  customDrawing();

  __asm {
    POPAD
    RETN 8
  }
}