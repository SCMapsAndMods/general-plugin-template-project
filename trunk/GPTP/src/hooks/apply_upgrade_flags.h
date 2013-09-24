#pragma once
#include "../SCBW/structures.h"

namespace hooks {

void applyUpgradeFlagsToNewUnitHook(CUnit* const unit);
void applyUpgradeFlagsToExistingUnitsHook(const u8 playerId, const u8 upgradeId);

void injectApplyUpgradeFlags();

} //hooks
