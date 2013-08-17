#pragma once
#include "../SCBW/structures.h"

namespace offsets {
const u32 Hook_UnitIsConsumable   = 0x00491F51;
const u32 Hook_OnConsumeUnit      = 0x004F47DE;
const u32 Util_IncrementDeathScores = 0x00488AF0;
}

bool unitIsConsumable(const CUnit* unit, u8 consumingPlayer);
void onConsumeUnit(CUnit* caster, CUnit *target);

//Inject with jmpPatch
static void __declspec(naked) unitIsConsumableWrapper() {
	CUnit* unit;
	u8 defilerOwner;
	__asm {
		PUSHAD
		MOV EBP, ESP
		MOV unit, EAX
		MOV defilerOwner, BL
	}

	if (unitIsConsumable(unit, defilerOwner)) {
		__asm {
			POPAD
			XOR AX, AX	;//Equivalent to MOV AX, 0
			POP EBP
			RETN 4
		}
	}
	else {
		__asm {
			POPAD
			MOV AX, 897	;//stat_txt.tbl string: Invalid target.<0>
			POP EBP
			RETN 4
		}
	}
}

//Inject with jmpPatch()
static void __declspec(naked) onConsumeUnitWrapper() {
	CUnit *caster, *target;

	__asm {
		PUSHAD
		MOV EBP, ESP
		MOV caster, ESI
		MOV target, EDI
	}

	onConsumeUnit(caster, target);

	__asm {
	  POPAD
	  POP EBX
	  POP EDI
	  RETN
	}
}

