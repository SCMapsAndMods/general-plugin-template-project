#pragma once
#include "../SCBW/structures.h"

namespace offsets {
const u32 Hook_UnitCanDetect  = 0x00403430;
}

bool unitCanDetectHook(const CUnit *unit);

//Inject with jmpPatch()
static void __declspec(naked) unitCanDetectWrapper() {
  CUnit *unit;
  u32 result;

  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, EAX
  }

  result = unitCanDetectHook(unit) ? 1 : 0;

  __asm {
    MOV EAX, result
    MOV [ESP + 28], EAX
    POPAD
    RETN
  }
}
