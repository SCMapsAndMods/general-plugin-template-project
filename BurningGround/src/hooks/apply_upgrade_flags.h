#pragma once
#include "../SCBW/structures.h"

namespace offsets {
const u32 Hook_ApplyUpgradeFlagsToNewUnit       = 0x00454370;
const u32 Hook_ApplyUpgradeFlagsToExistingUnits = 0x00454540;
}

void applyUpgradeFlagsToNewUnitHook(CUnit* const unit);
void applyUpgradeFlagsToExistingUnitsHook(const u8 playerId, const u8 upgradeId);


static void __declspec(naked) applyUpgradeFlagsToNewUnitWrapper() {
  static CUnit *unit;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, ESI
  }

  applyUpgradeFlagsToNewUnitHook(unit);

  __asm {
    POPAD
    RETN
  }
}

static void __declspec(naked) applyUpgradeFlagsToExistingUnitsWrapper() {
  static CUnit *unit;
  static u8 upgradeId;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV ECX, [EBP + 36]  ;//8 - 4 + 32
    MOV unit, ECX
    MOV upgradeId, AL
  }

  applyUpgradeFlagsToExistingUnitsHook(unit->playerId, upgradeId);

  __asm {
    POPAD
    RETN 4
  }
}
