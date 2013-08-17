#pragma once
#include "../SCBW/structures.h"

namespace offsets {
const u32 Hook_GetUnitMaxEnergy  = 0x00491870;
}

s32 getUnitMaxEnergyHook(const CUnit* const unit);

//Inject with jmpPatch()
static void __declspec(naked) getUnitMaxEnergyWrapper() {
  CUnit *unit;
  u32 maxEnergy;

  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, EAX
  }

  maxEnergy = getUnitMaxEnergyHook(unit);

  __asm {
    MOV EAX, maxEnergy
    MOV [ESP + 28], EAX
    POPAD
    RETN
  }
}

//Wrapper for native version
static s32 getUnitMaxEnergy(const CUnit* unit) {
  s32 result;
  __asm {
    PUSHAD
    MOV EAX, unit
    CALL offsets::Hook_GetUnitMaxEnergy
    MOV result, EAX
    POPAD
  }
  return result;
}
