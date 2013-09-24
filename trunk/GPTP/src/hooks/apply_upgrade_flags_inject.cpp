#include "apply_upgrade_flags.h"
#include "../hook_tools.h"

namespace hooks {

namespace offsets {
const u32 Hook_ApplyUpgradeFlagsToNewUnit       = 0x00454370;
const u32 Hook_ApplyUpgradeFlagsToExistingUnits = 0x00454540;
}

void __declspec(naked) applyUpgradeFlagsToNewUnitWrapper() {
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

void __declspec(naked) applyUpgradeFlagsToExistingUnitsWrapper() {
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

void injectApplyUpgradeFlags() {
  jmpPatch(applyUpgradeFlagsToNewUnitWrapper, 0x00454370);
  jmpPatch(applyUpgradeFlagsToExistingUnitsWrapper, 0x00454540);
}

} //hooks