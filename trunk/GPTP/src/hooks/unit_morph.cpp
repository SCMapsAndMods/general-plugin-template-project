#include <SCBW/scbwdata.h>
#include <SCBW/enumerations/TechId.h>

namespace hooks {

bool unitCanMorphHook(const CUnit *unit, u16 morphUnitId) {
  if (unit->id == UnitId::hydralisk) {
    if (morphUnitId == UnitId::lurker && unit->canUseTech(TechId::LurkerAspect, *ACTIVE_NATION_ID)
  }
  unit->id;
}

} //hooks
