#pragma once
#include "../SCBW/structures.h"
#include "../SCBW/offsets.h"

namespace offsets {
const u32 Hook_OrderNewUnitToRally        = 0x00466F62;
const u32 Hook_OrderNewUnitToRallyBack    = 0x00466FA3;
const u32 Hook_SetRallyPosition_Call      = 0x00456083;
const u32 Hook_SetRallyPosition_Jmp1      = 0x00456256;
const u32 Hook_SetRallyPosition_Jmp2      = 0x0049ACEF;
const u32 Hook_SetRallyUnit_Call          = 0x0045605F;
const u32 Hook_SetRallyUnit_Jmp1          = 0x0045620D;
const u32 Hook_SetRallyUnit_Jmp2          = 0x0049ACB0;
const u32 Hook_SetRallyGenericJmpBack1    = 0x00456384;
const u32 Hook_SetRallyGenericJmpBack2    = 0x0049AD77;
}

void orderNewUnitToRally(CUnit* unit, CUnit* factory);
void setRallyPosition(CUnit* unit, u16 x, u16 y);
void setRallyUnit(CUnit* unit, CUnit* target);

//Inject with jmpPatch()
static void __declspec(naked) orderNewUnitToRallyWrapper() {
  CUnit *unit, *factory;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, ESI
    MOV factory, ECX
  }

  orderNewUnitToRally(unit, factory);

  __asm {
    POPAD
    JMP [offsets::Hook_OrderNewUnitToRallyBack]
  }
}

//Inject with callPatch()
static void __declspec(naked) setRallyPositionWrapper_Call() {
  CUnit *unit;
  u16 x, y;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, EAX
    MOV y, DI
    MOV x, DX
  }

  setRallyPosition(unit, x, y);

  __asm {
    POPAD
    RETN 8
  }
}

//Inject with jmpPatch()
static void __declspec(naked) setRallyPositionWrapper_Jmp1() {
  CUnit *unit;
  u16 x, y;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, EDI
    MOV x, BX
    MOV y, SI
  }

  setRallyPosition(unit, x, y);

  __asm {
    POPAD
    JMP [offsets::Hook_SetRallyGenericJmpBack1]
  }
}

//Inject with jmpPatch()
static void __declspec(naked) setRallyPositionWrapper_Jmp2() {
  CUnit *unit;
  u16 x, y;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, ESI
    MOV x, CX
    MOV y, DX
  }

  setRallyPosition(unit, x, y);

  __asm {
    POPAD
    JMP [offsets::Hook_SetRallyGenericJmpBack2]
  }
}

//Inject with callPatch()
static void __declspec(naked) setRallyUnitWrapper_Call() {
  CUnit *unit, *target;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, ECX
    MOV target, EAX
  }

  setRallyUnit(unit, target);

  __asm {
    POPAD
    RETN
  }
}

//Inject with jmpPatch()
static void __declspec(naked) setRallyUnitWrapper_Jmp1() {
  CUnit *unit, *target;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, EDI
    MOV target, EAX
  }

  setRallyUnit(unit, target);

  __asm {
    POPAD
    JMP [offsets::Hook_SetRallyGenericJmpBack1]
  }
}

//Inject with jmpPatch()
static void __declspec(naked) setRallyUnitWrapper_Jmp2() {
  CUnit *unit, *target;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, ESI
    MOV target, EBX
  }

  setRallyUnit(unit, target);

  __asm {
    POPAD
    JMP [offsets::Hook_SetRallyGenericJmpBack2]
  }
}
