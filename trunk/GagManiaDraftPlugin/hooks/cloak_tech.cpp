#include "cloak_tech.h"
#include "../SCBW/enumerations/UnitId.h"
#include "../SCBW/enumerations/TechId.h"
#include <definitions.h>

namespace hooks {

//Returns the tech ID used by this unit for the cloaking spell.
//For the cloaking energy consumption, see energy_regeneration.cpp
u8 getCloakingTech(const CUnit *unit) {

  if (unit->id == UnitId::ghost
    || unit->id == UnitId::sarah_kerrigan
    || unit->id == UnitId::Hero_AlexeiStukov
    || unit->id == UnitId::Hero_SamirDuran
    || unit->id == UnitId::Hero_InfestedDuran
    || unit->id == UnitId::Hero_InfestedKerrigan)
    return TechId::PersonnelCloaking;

  if (unit->id == UnitId::wraith || unit->id == UnitId::tom_kazansky)
    return TechId::CloakingField;

  //스카웃 클로킹 설정
  if (unit->id == UnitId::scout)
    return TECH_SCOUT_CLOAKING;

  return TechId::None;
}

} //hooks
