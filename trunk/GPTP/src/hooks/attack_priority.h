#include <SCBW/structures/CUnit.h>

namespace hooks {

u32 getAttackPriorityHook(const CUnit* unit, const CUnit* target);
const CUnit* findBestAttackTargetHook(CUnit* unit);
const CUnit* findRandomAttackTargetHook(CUnit* unit);

void injectAttackPriorityHooks();

} //hooks
