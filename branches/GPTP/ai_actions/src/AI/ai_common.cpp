#include "ai_common.h"
#include "../SCBW/enumerations.h"
#include "../SCBW/api.h"
#include "../SCBW/UnitFinder.h"

namespace AI {

bool isTargetWorthHitting(const CUnit *unit, const CUnit *target) {
  //If the target is hidden by the fog-of-war
  if (!target->sprite->isVisibleTo(unit->playerId))
    return false;

  //If the target is not detectable
  if (target->status & (UnitStatus::Cloaked | UnitStatus::RequiresDetection)
      && !target->isVisibleTo(unit->playerId))
    return false;

  const s8 targetOwner = target->getLastOwnerId();

  if (scbw::isAlliedTo(unit->playerId, targetOwner))
    return false;

  if (target->id == UnitId::ProtossScarab
      || target->id == UnitId::spider_mine
      || target->id == UnitId::interceptor)
    return false;

  //Target is a worker inside a gas building
  if (target->mainOrderId == OrderId::HarvestGas1
      || target->mainOrderId == OrderId::HarvestGas2
      || target->mainOrderId == OrderId::HarvestGas3)
    return false;

  return true;
}

//-------- Unit stat accumulators --------//

scbw::UnitFinder unitStatTotalFinder;

class UnitLifeAccumulatorProc {
  private:
    CUnit *caster;
    int sum;
  public:
    void operator()(const CUnit *target);
};

void UnitLifeAccumulatorProc::operator()(const CUnit *target) {
  if (target->status & UnitStatus::Invincible)
    return;

  if (scbw::isAlliedTo(caster->playerId, target->playerId))
    return;

  sum += ((target->hitPoints + 255) / 256) + (target->shields / 256);
}

int getTotalEnemyHpInArea(int x, int y, int searchBounds, const CUnit *caster) {
  unitStatTotalFinder.search(x - searchBounds, y - searchBounds,
                             x + searchBounds, y + searchBounds);
  
}

int getTotalEnemyShieldsInArea(int x, int y, int searchBounds, const CUnit *caster);
int getTotalEnemyEnergyInArea(int x, int y, int searchBounds, const CUnit *caster);

int getTotalAllyHpInArea(int x, int y, int searchBounds, const CUnit *caster);
int getTotalAllyShieldsInArea(int x, int y, int searchBounds, const CUnit *caster);


} //AI
