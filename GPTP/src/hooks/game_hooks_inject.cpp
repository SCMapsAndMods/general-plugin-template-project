#include "game_hooks.h"
#include "../hook_tools.h"

namespace hooks {

bool isGameOn = false;

/******** Game Start ********/

const u32 Func_CompileSCode = 0x00417C20;
void __declspec(naked) onGameStart() { // From BWAPI by Kovarex, slightly modified
	__asm PUSHAD;

	{
		isGameOn = true;
    gameOn();
	}
	__asm {
		POPAD
		CALL Func_CompileSCode
		RETN
	}
}

/******** Game End ********/

const u32 Func_DeleteSCode = 0x00416D90;
void __declspec(naked) onGameEnd() { // From BWAPI by Kovarex, slightly modified
	__asm PUSHAD;
	{
		isGameOn = false;
    gameEnd();
	}
	__asm {
		POPAD
		call Func_DeleteSCode
		RETN
	}
}

/******** NEXT FRAME HOOK ********/

const u32 Func_IsGamePaused = 0x00488780;
void __declspec(naked) nextFrameHook() { // From BWAPI by Kovarex, slightly modified
	__asm {
		CALL Func_IsGamePaused
		PUSHAD
		MOV EBP, ESP
	}
	{
    nextFrame();
	}
	__asm {
		POPAD
		RETN
	}
}

//-------- Hook njector --------//

void injectGameHooks() {
  callPatch(onGameStart,    0x004C9A51);
  callPatch(onGameEnd,      0x004EE983);
  callPatch(nextFrameHook,  0x004D974E);
}

} //hooks
