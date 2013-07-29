#pragma once
#include "../SCBW/structures.h"

namespace offsets {
const u32 Hook_UseStimPacksAi         = 0x00440148;
const u32 Hook_UseStimPacksPlayer     = 0x004C2F68;
const u32 Hook_UseStimPacksPlayerBack = 0x004C2FF9;
const u32 Hook_CanUseStimPacks        = 0x004234E6;
const u32 Hook_CanUseStimPacksYes     = 0x00423528;
const u32 Hook_CanUseStimPacksNo      = 0x004234EB;
}

void useStimPacksHook(CUnit *unit);
bool canUseStimPacksHook(const CUnit *unit);

//Inject with callPatch
static void __declspec(naked) useStimPacksAiWrapper() {
  CUnit *unit;

  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, EAX
  }

  useStimPacksHook(unit);

  __asm {
    POPAD
    RETN
  }
}

//Inject with jmpPatch
static void __declspec(naked) useStimPacksPlayerWrapper() {
  CUnit *unit;

  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, ESI
  }

  useStimPacksHook(unit);

  __asm {
    POPAD
    JMP offsets::Hook_UseStimPacksPlayerBack
  }
}

//Inject with jmpPatch
static void __declspec(naked) canUseStimPacksWrapper() {
  CUnit *unit;

  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, EAX
  }

  if (canUseStimPacksHook(unit)) {
    __asm {
      POPAD
      JMP offsets::Hook_CanUseStimPacksYes
    }
  }
  else {
    __asm {
      POPAD
      JMP offsets::Hook_CanUseStimPacksNo
    }

  }
}
