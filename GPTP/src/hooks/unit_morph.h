#include <SCBW/structures/CUnit.h>

namespace hooks {

bool unitCanMorphHook(const CUnit *unit, u16 morphUnitId);
bool isEggUnitHook(u16 unitId);
u16 getUnitMorphEggTypeHook(u16 unitId);
u16 getCancelMorphRevertTypeHook(const CUnit *eggUnit);
bool hasSuppliesForUnitHook(s8 playerId, u16 unitId, bool canShowErrorMessage);

void injectUnitMorphHooks();

} //hooks
