#pragma once
#include "../SCBW/structures.h"

namespace offsets {
const u32 Hook_GetSeekRange       = 0x00476000;
const u32 Hook_GetMaxWeaponRange  = 0x00475870;
}

u8 getSeekRangeHook(const CUnit *unit);
u32 getMaxWeaponRangeHook(const CUnit *unit, u8 weaponId);

//Inject with jmpPatch()
static void __declspec(naked) getSeekRangeWrapper() {
  CUnit *unit;
  u8 seekRange;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, EDX
  }

  seekRange = getSeekRangeHook(unit);

  __asm {
    MOVZX EAX, seekRange
    MOV [ESP + 28], EAX
    POPAD
    RETN
  }
}

//Inject with jmpPatch()
static void __declspec(naked) getMaxWeaponRangeWrapper() {
  CUnit *unit;
  u8 weaponId;
  u32 maxWeaponRange;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, EAX
    MOV weaponId, BL
  }

  maxWeaponRange = getMaxWeaponRangeHook(unit, weaponId);

  __asm {
    MOV EAX, maxWeaponRange;
    MOV [ESP + 28], EAX
    POPAD
    RETN
  }
}
