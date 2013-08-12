#pragma once
#include "../SCBW/structures.h"

namespace offsets {
const u32 Hook_UnitCanAttackInsideBunker    = 0x004790A3;
const u32 Hook_UnitCanAttackInsideBunkerYes = 0x004790DC;
}

bool unitCanAttackInsideBunkerHook(const CUnit *unit);

static void __declspec(naked) unitCanAttackInsideBunkerWrapper() {
  CUnit *unit;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, ESI
  }

  if (unitCanAttackInsideBunkerHook(unit)) {
    __asm {
      POPAD
      JMP offsets::Hook_UnitCanAttackInsideBunkerYes
    }
  }
  else {
    __asm {
      POPAD
      POP ESI   ;//Code at 0x00479147
      RETN
    }
  }
}
