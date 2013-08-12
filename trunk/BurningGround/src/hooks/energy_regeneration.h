#pragma once
#include "../SCBW/structures.h"

namespace offsets {
const u32 Hook_RegenerateEnergy = 0x004EB4B0;
}

void regenerateEnergyHook(CUnit *unit);

//Inject with jmpPatch()
static void __declspec(naked) regenerateEnergyWrapper() {
  CUnit *unit;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, ESI
  }

  regenerateEnergyHook(unit);

  __asm {
    POPAD
    RETN
  }
}

//Wrapper for native version of regenerateEnergy()
static void regenerateEnergy(CUnit *unit) {
  __asm {
    PUSHAD
    MOV ESI, unit
    CALL offsets::Hook_RegenerateEnergy
    POPAD
  }
}
