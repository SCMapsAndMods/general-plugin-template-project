#pragma once
#include "../SCBW/structures.h"
#include "../tools.h"


namespace hooks {

void cloakNearbyUnitsHook(CUnit *unit);

}

static void __declspec(naked) cloakNearbyUnitsWrapper() {
  CUnit *unit;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, EDI 
  }

  hooks::cloakNearbyUnitsHook(unit);

  __asm {
    POPAD
    RETN
  }
}

static void injectCloakNearbyUnitsHook() {
  jmpPatch(cloakNearbyUnitsWrapper, 0x00491C20);
}
