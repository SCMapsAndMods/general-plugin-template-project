#pragma once
#include "../SCBW/structures.h"

namespace offsets {
const u32 Hook_GetUnitMaxEnergy  = 0x00491870;
}

u16 getUnitMaxEnergyHook(const CUnit* const unit);

//Inject with jmpPatch()
static void __declspec(naked) getUnitMaxEnergyWrapper() {
  static CUnit *unit;
  static u16 maxEnergy;

  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, EAX
  }

  maxEnergy = getUnitMaxEnergyHook(unit);

  __asm {
    POPAD
    MOVZX EAX, maxEnergy
    RETN
  }
}

//Wrapper for native version
static u16 getUnitMaxEnergy(const CUnit* unit) {
  static u16 result;
  __asm {
    PUSHAD
    MOV EAX, unit
    CALL offsets::Hook_GetUnitMaxEnergy
    MOV result, AX
    POPAD
  }
  return result;
}
