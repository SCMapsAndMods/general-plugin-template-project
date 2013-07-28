#pragma once
#include "../SCBW/structures.h"

namespace offsets {
const u32 Hook_GetModifiedWeaponCooldown  = 0x00475DC0;
}

u32 getModifiedWeaponCooldownHook(const CUnit* unit, u8 weaponId);

//Inject with jmpPatch()
static void __declspec(naked) getModifiedWeaponCooldownWrapper() {
	CUnit *unit;
	u8 weaponId;
	u32 cooldown;
	__asm {
		PUSHAD
		MOV ebp, esp
		MOV unit, esi
		MOV weaponId, al
	}

	cooldown = getModifiedWeaponCooldownHook(unit, weaponId);

	__asm {
		MOV eax, cooldown
		MOV [esp + 28], eax
		POPAD
		RETN
	}
}
