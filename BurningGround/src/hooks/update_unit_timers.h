#pragma once
#include "../SCBW/structures.h"

namespace offsets {
const u32 Hook_UpdateUnitTimers   = 0x004EC290;
}

void updateUnitTimersHook(CUnit* unit);

//Inject with jmpPatch()
static void __declspec(naked) updateUnitTimersWrapper() {
  CUnit *unit;

  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, EAX
  }

  updateUnitTimersHook(unit);

  __asm {
    POPAD
    RETN
  }
}
