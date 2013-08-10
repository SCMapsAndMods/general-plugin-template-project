// How to access registers stored with PUSHAD:
// EAX => [ESP + 28]
// ECX => [ESP + 24]
// EDX => [ESP + 20]
// EBX => [ESP + 16]
// EBP => [ESP + 12]
// ESP => [ESP + 8] (warning: reading/modifying the value of ESP is meaningless!)
// ESI => [ESP + 4]
// EDI => [ESP]

#pragma once
#include "../types.h"

static u8 game_on = 0;

namespace offsets {
const u32 GameStart             = 0x004C9A51;
const u32 GameStartBack         = GameStart + 5;
const u32 GameStartTarget       = 0x00417C20;
const u32 GameEnd               = 0x004EE983;
const u32 GameEndBack           = GameEnd + 5;
const u32 GameEndTarget         = 0x00416D90;
const u32 NextLogicFrame        = 0x004D974E;
const u32 NextLogicFrameBack    = NextLogicFrame + 5;
const u32 NextLogicFrameTarget  = 0x00488780;
}

bool gameOn();
bool gameEnd();
bool nextFrame();

/******** Game Start ********/

static void __declspec(naked) onGameStart() { // From BWAPI by Kovarex, slightly modified
	__asm PUSHAD;

	{
		game_on = 1;
    gameOn();
	}
	__asm {
		POPAD
		call [offsets::GameStartTarget]
		jmp [offsets::GameStartBack]
	}
}


/******** Game End ********/

static void __declspec(naked) onGameEnd() { // From BWAPI by Kovarex, slightly modified
	__asm PUSHAD;
	{
		game_on = 0;
    gameEnd();
	}
	__asm {
		POPAD
		call [offsets::GameEndTarget]
		jmp [offsets::GameEndBack]
	}
}


/******** NEXT FRAME HOOK ********/

static void __declspec(naked) nextFrameHook() { // From BWAPI by Kovarex, slightly modified
	__asm {
		CALL [offsets::NextLogicFrameTarget]
		PUSHAD
		MOV EBP, ESP
	}
	{
    nextFrame();
	}
	__asm {
		POPAD
		JMP [offsets::NextLogicFrameBack]
	}
}

