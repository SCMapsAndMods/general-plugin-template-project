#pragma once
#include "../SCBW/structures.h"


void doWeaponDamageHook(s32     damage,
                        CUnit*  target,
                        u8      weaponId,
                        CUnit*  attacker,
                        u32     attackingPlayer,
                        u8      direction,
                        u32     dmgDivisor
                        );

//Inject with JmpPatch
static void __declspec(naked) doWeaponDamageWrapper() {
  static CUnit *target, *attacker;
  static s32 damage;
  static u32 attackingPlayer, dmgDivisor;
  static u8 weaponId, direction;

  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV damage, EAX
    MOV target, EDI
    MOV EBX, [ESP + 36]   ;// (PUSHAD saves 32) + (CALL saves 4) == 36
    MOV weaponId, BL      ;// Use EBX as a "temp" variable for copying args
    MOV EBX, [ESP + 40]
    MOV dmgDivisor, EBX
    MOV EBX, [ESP + 44]
    MOV direction, BL
    MOV EBX, [ESP + 48]
    MOV attacker, EBX
    MOV EBX, [ESP + 52]
    MOV attackingPlayer, EBX
  }

  doWeaponDamageHook(damage, target, weaponId, attacker, attackingPlayer,
                     direction, dmgDivisor);

  __asm {
    POPAD
    RETN 20
  }
}
