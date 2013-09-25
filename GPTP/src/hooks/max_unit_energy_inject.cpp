#include "max_unit_energy.h"
#include "../hook_tools.h"

namespace {

//Inject with jmpPatch()
static void __declspec(naked) getUnitMaxEnergyWrapper() {
  static CUnit *unit;
  static u16 maxEnergy;

  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, EAX
  }

  maxEnergy = hooks::getUnitMaxEnergyHook(unit);

  __asm {
    POPAD
    MOVZX EAX, maxEnergy
    RETN
  }
}

} //unnamed namespace

namespace hooks {

void injectUnitMaxEnergyHook() {
  jmpPatch(getUnitMaxEnergyWrapper, 0x00491870);
}

} //hooks
