#include "spellcasting.h"
#include "spells/dark_swarm.h"

//-------- Helper function declarations. Do NOT modify! --------//
namespace {

bool canCastSpell(const CUnit *unit, u16 techId, u8 orderId);

} //unnamed namespace

namespace AI {

//Attempts make the @p unit cast a spell.
bool AI_spellcaster(CUnit *unit, bool isUnitBeingAttacked) {
  if (!isUnitBeingAttacked
      && AIScriptController[unit->playerId].spellcasterTimer != 0)
    return false;

  switch (unit->id) {
    case UnitId::science_vessel:
      break;

    case UnitId::battlecruiser:
      break;

    case UnitId::ghost:
      break;

    case UnitId::medic:
      break;

    case UnitId::queen:
      break;

    case UnitId::defiler:
      break;

    case UnitId::high_templar:
      break;

    case UnitId::arbiter:
      break;

    case UnitId::dark_archon:
      break;

    case UnitId::corsair:
      break;
  }

  return false;
}

} //AI


//-------- Helper function declarations. Do NOT modify! --------//
namespace {

bool canCastSpell(const CUnit *unit, u16 techId, u8 orderId) {
  //TODO: Add content
}

} //unnamed namespace
