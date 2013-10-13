#include "damage_with.h"
#include "../hook_tools.h"

extern const u32 Func_DamageWith; //Defined in CUnit.cpp

namespace {

//Inject with JmpPatch
void __declspec(naked) damageWithWrapper() {
  static CUnit *target, *attacker;
  static s32 damage;
  static u8 weaponId, damageDivisor;
  static s8 attackingPlayer, direction;

  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV damage, EAX
    MOV target, EDI
    MOV EBX, [ESP + 36]   ;// (PUSHAD saves 32) + (CALL saves 4) == 36
    MOV weaponId, BL      ;// Use EBX as a "temp" variable for copying args
    MOV EBX, [ESP + 40]
    MOV damageDivisor, BL
    MOV EBX, [ESP + 44]
    MOV direction, BL
    MOV EBX, [ESP + 48]
    MOV attacker, EBX
    MOV EBX, [ESP + 52]
    MOV attackingPlayer, BL
  }

  hooks::damageWithHook(damage, target, weaponId, attacker, attackingPlayer,
                        direction, damageDivisor);

  __asm {
    POPAD
    RETN 20
  }
}

} //unnamed namespace

namespace hooks {

void injectDamageWithHook() {
  jmpPatch(damageWithWrapper, Func_DamageWith);
}

} //hooks
