#include "ai_common.h"

namespace AI {

//
bool isTargetWorthHitting(CUnit *unit, CUnit *target);

int getTotalEnemyHpInArea(int x, int y, int searchBounds, const CUnit *unit);
int getTotalEnemyShieldsInArea(int x, int y, int searchBounds, const CUnit *unit);
int getTotalEnemyEnergyInArea(int x, int y, int searchBounds, const CUnit *unit);

int getTotalAllyHpInArea(int x, int y, int searchBounds, const CUnit *unit);
int getTotalAllyShieldsInArea(int x, int y, int searchBounds, const CUnit *unit);


} //AI
