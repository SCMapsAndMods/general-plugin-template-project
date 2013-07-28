#pragma once
#include "../SCBW/structures.h"

namespace offsets {
const u32 Hook_GetSightRange  = 0x004E5B40;
}

u32 getSightRangeHook(const CUnit *unit, bool isForSpellCasting);

static void __declspec(naked) getSightRangeWrapper() {
  CUnit *unit;
  u32 isForSpellCasting, sightRange;

  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, EDX
    MOV EAX, DWORD PTR [EBP + 36]
    MOV isForSpellCasting, EAX
  }

  sightRange = getSightRangeHook(unit, isForSpellCasting != 0);

  __asm {
    MOV EAX, sightRange
    MOV [ESP + 28], EAX
    POPAD
    RETN 4
  }
}
