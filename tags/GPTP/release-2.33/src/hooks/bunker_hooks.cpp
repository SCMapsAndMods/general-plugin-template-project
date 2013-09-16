#include "bunker_hooks.h"
#include "../SCBW/enumerations.h"

/// Checks whether the unit can attack from inside a bunker.
bool unitCanAttackInsideBunkerHook(const CUnit *unit) {
  //Default StarCraft behavior
  const u16 unitId = unit->id;
  if (unitId == UnitId::TerranMarine
      || unitId == UnitId::Hero_JimRaynorMarine
      || unitId == UnitId::TerranGhost
      || unitId == UnitId::Hero_SarahKerrigan
      || unitId == UnitId::Hero_AlexeiStukov
      || unitId == UnitId::Hero_SamirDuran
      || unitId == UnitId::Hero_InfestedDuran
      || unitId == UnitId::TerranFirebat
      || unitId == UnitId::Hero_GuiMontag)
    return true;
  else
    return false;
}
