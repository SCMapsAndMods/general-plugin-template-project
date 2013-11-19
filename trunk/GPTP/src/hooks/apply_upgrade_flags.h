//Header file for the Apply Upgrade Flags hook module.
#pragma once
#include <SCBW/structures.h>

namespace hooks {

void applyUpgradeFlagsToNewUnitHook(CUnit *unit);
void applyUpgradeFlagsToExistingUnitsHook(u8 playerId, u8 upgradeId);

void injectApplyUpgradeFlags();

//Wrapper for the native version of applyUpgradeFlagsToExistingUnits()
void applyUpgradeFlagsToExistingUnits(const CUnit *unit, u8 upgradeId);

} //hooks
