#pragma once
#include "../SCBW/structures.h"

namespace offsets {
const u32 Hook_GetArmorBonus  = 0x00453FC0;
}

u8 getArmorBonusHook(const CUnit *unit);

//Inject with jmpPatch()
static void __declspec(naked) getArmorBonusWrapper() {
  CUnit *unit;
  u8 armor;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, EAX
  }

  armor = getArmorBonusHook(unit);

  __asm {
    MOVZX EAX, armor
    MOV [ESP + 28], EAX
    POPAD
    RETN
  }
}

//Wrapper for native version
static u8 getArmorBonus(const CUnit *unit) {
  u8 armor;
  __asm {
    PUSHAD
    MOV EAX, unit
    CALL offsets::Hook_GetArmorBonus
    MOV armor, AL
    POPAD
  }
  return armor;
}
