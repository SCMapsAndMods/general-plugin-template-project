#include "energy_regeneration.h"
#include "../hook_tools.h"

namespace {

//Inject with jmpPatch()
const u32 Hook_RegenerateEnergy = 0x004EB4B0;
void __declspec(naked) regenerateEnergyWrapper() {
  CUnit *unit;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, ESI
  }

  hooks::regenerateEnergyHook(unit);

  __asm {
    POPAD
    RETN
  }
}

} //unnamed namespace

namespace hooks {

void injectEnergyRegenerationHook() {
  jmpPatch(regenerateEnergyWrapper, Hook_RegenerateEnergy);
}

void regenerateEnergy(CUnit *unit) {
  __asm {
    PUSHAD
    MOV ESI, unit
    CALL Hook_RegenerateEnergy
    POPAD
  }
}

} //hooks
