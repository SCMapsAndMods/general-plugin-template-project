#pragma once
#include "../SCBW/structures.h"

namespace offsets {
const u32 Hook_RechargeShieldsProc        = 0x00493ED2;
const u32 Hook_UnitCanRechargeShields     = 0x00493520;
const u32 Hook_CanStopRechargeShields     = 0x00493ED7;
const u32 Hook_CanStopRechargeShieldsYes  = 0x00493EF7;
}

void rechargeShieldsProcHook(CUnit *target, CUnit *battery);
bool unitCanRechargeShieldsHook(const CUnit *target, const CUnit *battery);
bool canStopRechargeShieldsHook(const CUnit *target, const CUnit *battery);

//Inject with callPatch
static void __declspec(naked) rechargeShieldsProcWrapper() {
  CUnit *target, *battery;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV battery, EBX
    MOV target, ECX
  }

  rechargeShieldsProcHook(target, battery);

  __asm {
    POPAD
    RETN
  }
}

//Inject with jmpPatch
static void __declspec(naked) unitCanRechargeShieldsWrapper() {
  CUnit *target, *battery;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV battery, EDI
    MOV target, EAX
  }

  if (unitCanRechargeShieldsHook(target, battery)) {
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
static void __declspec(naked) canStopRechargeShieldsWrapper() {
  CUnit *target, *battery;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV battery, EBX
    MOV target, EDI
  }

  if (canStopRechargeShieldsHook(target, battery)) {
    __asm {
      POPAD
      JMP offsets::Hook_CanStopRechargeShieldsYes
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
