#include <SCBW/structures/CUnit.h>

namespace hooks {

bool isMorphedBuildingHook(u16 unitId);
int getMorphBuildingTypeCountHook(const CUnit *unit, u16 unitId, bool ignoreIncomplete);

void injectBuildingMorphHooks();

//Wrapper for the hook of isMorphingBuilding()
bool isMorphingBuildingHook(const CUnit *unit);

} //hooks
