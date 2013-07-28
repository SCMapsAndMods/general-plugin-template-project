#pragma once
#include "../SCBW/structures.h"

namespace offsets {
const u32 Hook_UpdateStatusEffects  = 0x00492F70;
}

void updateStatusEffectsHook(CUnit *unit);

//Inject with jmpPatch()
static void __declspec(naked) updateStatusEffectsWrapper() {
  CUnit *unit;

  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, EAX
  }

  updateStatusEffectsHook(unit);

  __asm {
    POPAD
    RETN
  }
}

//Wrapper for native version of updateStatusEffects()
static void updateStatusEffects(CUnit *unit) {
  __asm {
    PUSHAD
    MOV EAX, unit
    CALL offsets::Hook_UpdateStatusEffects
    POPAD
  }
}
