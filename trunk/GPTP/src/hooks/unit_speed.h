#pragma once
#include "../SCBW/structures.h"

namespace offsets {
const u32 Hook_GetModifiedUnitSpeed         = 0x0047B5F0;
const u32 Hook_GetModifiedUnitAcceleration  = 0x0047B8A0;
const u32 Hook_GetModifiedUnitTurnSpeed     = 0x0047B850;
}

u32 getModifiedUnitSpeedHook(const CUnit* unit, u32 baseSpeed);
u32 getModifiedUnitAccelerationHook(const CUnit* unit);
u32 getModifiedUnitTurnSpeedHook(const CUnit* unit);

//Inject with jmpPatch()
static void __declspec(naked) getModifiedUnitSpeedWrapper() {
	CUnit *unit;
	u32 speed;
	__asm {
		PUSHAD
		MOV ebp, esp
		MOV unit, edx
		MOV speed, eax
	}

	speed = getModifiedUnitSpeedHook(unit, speed);

	__asm {
		MOV eax, speed
		MOV [esp + 28], eax
		POPAD
		RETN
	}
}

//Inject with jmpPatch()
static void __declspec(naked) getModifiedUnitAccelerationWrapper() {
	CUnit *unit;
	u32 acceleration;
	__asm {
		PUSHAD
		MOV ebp, esp
		MOV unit, ecx
	}

	acceleration = getModifiedUnitAccelerationHook(unit);

	__asm {
		MOV eax, acceleration
		MOV [esp + 28], eax
		POPAD
		RETN
	}
}

//Inject with jmpPatch()
static void __declspec(naked) getModifiedUnitTurnSpeedWrapper() {
	CUnit *unit;
	u32 turnSpeed;
	__asm {
		PUSHAD
		MOV ebp, esp
		MOV unit, ecx
	}

	turnSpeed = getModifiedUnitTurnSpeedHook(unit);

	__asm {
		MOV eax, turnSpeed
		MOV [esp + 28], eax
		POPAD
		RETN
	}
}
