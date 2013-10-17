#include "bunker_hooks.h"
#include "../hook_tools.h"

namespace {

const u32 Hook_UnitCanAttackInsideBunkerYes = 0x004790DC;
void __declspec(naked) unitCanAttackInsideBunkerWrapper() {
  CUnit *unit;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, ESI
  }

  if (hooks::unitCanAttackInsideBunkerHook(unit)) {
    __asm {
      POPAD
      JMP Hook_UnitCanAttackInsideBunkerYes
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

} //unnamed namespace

namespace hooks {

void injectBunkerHooks() {
  jmpPatch(unitCanAttackInsideBunkerWrapper, 0x004790A3);
}

} //hooks