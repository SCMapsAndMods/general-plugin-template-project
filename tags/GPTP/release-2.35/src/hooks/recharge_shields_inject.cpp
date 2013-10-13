#include "recharge_shields.h"
#include "../hook_tools.h"

namespace {

//Inject with callPatch
const u32 Hook_RechargeShieldsProc = 0x00493ED2;
void __declspec(naked) rechargeShieldsProcWrapper() {
  CUnit *target, *battery;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV battery, EBX
    MOV target, ECX
  }

  hooks::rechargeShieldsProcHook(target, battery);

  __asm {
    POPAD
    RETN
  }
}

//Inject with jmpPatch
const u32 Hook_UnitCanRechargeShields = 0x00493520;
void __declspec(naked) unitCanRechargeShieldsWrapper() {
  CUnit *target, *battery;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV battery, EDI
    MOV target, EAX
  }

  if (hooks::unitCanRechargeShieldsHook(target, battery)) {
    __asm {
      POPAD
      MOV EAX, 1
      RETN
    }
  }
  else {
    __asm {
      POPAD
      XOR EAX, EAX  ;//Identical to MOV EAX, 0
      RETN
    }
  }
}

//Inject with jmpPatch
const u32 Hook_CanStopRechargeShields = 0x00493ED7;
const u32 Hook_CanStopRechargeShieldsYes = 0x00493EF7;
void __declspec(naked) canStopRechargeShieldsWrapper() {
  CUnit *target, *battery;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV battery, EBX
    MOV target, EDI
  }

  if (hooks::canStopRechargeShieldsHook(target, battery)) {
    __asm {
      POPAD
      JMP Hook_CanStopRechargeShieldsYes
    }
  }
  else {
    __asm {
      POPAD
      POP ESI
      POP EBX
      RETN
    }
  }
}

} //unnamed namespace

namespace hooks {

void injectRechargeShieldsHooks() {
  callPatch(rechargeShieldsProcWrapper,   Hook_RechargeShieldsProc);
  jmpPatch(unitCanRechargeShieldsWrapper, Hook_UnitCanRechargeShields);
  jmpPatch(canStopRechargeShieldsWrapper, Hook_CanStopRechargeShields);
}

} //hooks
