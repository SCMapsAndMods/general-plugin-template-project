//Header file for the Transfer Tech & Upgrades hook module.
#include <SCBW/structures/CUnit.h>

namespace hooks {

void transferUnitTechToPlayerHook(const CUnit *source, u8 targetPlayerId);
void transferUnitUpgradesToPlayerHook(const CUnit *source, u8 targetPlayerId);
void applyUnitUpgradeFlagsToAllFriendlyUnitsHook(CUnit *unit);

void injectTransferTechAndUpgradesHooks();

} //hooks
