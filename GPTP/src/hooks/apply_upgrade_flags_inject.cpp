#include "apply_upgrade_flags.h"
#include "../hook_tools.h"

void __declspec(naked) applyUpgradeFlagsToNewUnitWrapper() {
  static CUnit *unit;
  __asm {
    PUSHAD
    MOV EBP, ESP
    MOV unit, ESI
  }

  hooks::applyUpgradeFlagsToNewUnitHook(unit);

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

  hooks::applyUpgradeFlagsToExistingUnitsHook(unit->playerId, upgradeId);

  __asm {
    POPAD
    RETN 4
  }
}

namespace hooks {

void injectApplyUpgradeFlags() {
  jmpPatch(applyUpgradeFlagsToNewUnitWrapper, 0x00454370);
  jmpPatch(applyUpgradeFlagsToExistingUnitsWrapper, 0x00454540);
}

} //hooks