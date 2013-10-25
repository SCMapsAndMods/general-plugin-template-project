#include "spellcasting.h"
#include "spells/dark_swarm.h"
#include "spells/yamato_gun.h"

#include "spells/psi_storm.h"
#include "spells/hallucination.h"

#include "spells/stasis_field.h"
#include "spells/recall.h"

#include "spells/disruption_web.h"

//-------- Helper function declarations. Do NOT modify! --------//
namespace {

bool canCastSpellOrder(const CUnit *unit, u8 techId, u8 orderId);
bool aiCastSpellOrder(CUnit *unit, CUnit *target, u8 orderId, u8 aiActionFlag = 1);

} //unnamed namespace

namespace AI {

//Attempts make the @p unit cast a spell.
bool AI_spellcasterHook(CUnit *unit, bool isUnitBeingAttacked) {
  if (!isUnitBeingAttacked
      && AIScriptController[unit->playerId].spellcasterTimer != 0)
    return false;

  switch (unit->id) {
    case UnitId::science_vessel:
      break;

    case UnitId::battlecruiser:
      if (canCastSpellOrder(unit, TechId::YamatoGun, OrderId::FireYamatoGun1)) {
        CUnit *target = findBestYamatoGunTarget(unit, isUnitBeingAttacked);
        
        if (unit->mainOrderId == OrderId::FireYamatoGun1
            && unit->orderTarget.unit == target)
          return false;

        if (aiCastSpellOrder(unit, target, OrderId::FireYamatoGun1))
          return true;
      }

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
      if (canCastSpellOrder(unit, TechId::PsionicStorm, OrderId::PsiStorm)) {
        CUnit *target = findBestPsiStormTarget(unit, isUnitBeingAttacked);
        
        if (unit->mainOrderId == OrderId::PsiStorm
            && unit->orderTarget.unit == target)
          return false;

        if (aiCastSpellOrder(unit, target, OrderId::PsiStorm))
          return true;
      }

      if (unit->getMaxEnergy() != unit->energy
          && canCastSpellOrder(unit, TechId::Hallucination, OrderId::Hallucianation1)) {
        CUnit *target = findBestHallucinationTarget(unit, isUnitBeingAttacked);

        if (unit->mainOrderId == OrderId::Hallucianation1
            && unit->orderTarget.unit == target)
          return false;

        if (aiCastSpellOrder(unit, target, OrderId::Hallucianation1))
          return true;
      }

      break;

    case UnitId::arbiter:
      if (canCastSpellOrder(unit, TechId::StasisField, OrderId::StasisField)) {
        CUnit *target = findBestStasisFieldTarget(unit, isUnitBeingAttacked);
        
        if (unit->mainOrderId == OrderId::StasisField
            && unit->orderTarget.unit == target)
          return false;

        if (aiCastSpellOrder(unit, target, OrderId::StasisField))
          return true;
      }
      
      if (canCastSpellOrder(unit, TechId::Recall, OrderId::Teleport)) {
        CUnit *target = findBestStasisFieldTarget(unit, isUnitBeingAttacked);
        
        if (unit->mainOrderId == OrderId::Teleport
            && unit->orderTarget.unit == target)
          return false;

        if (aiCastSpellOrder(unit, target, OrderId::Teleport))
          return true;
      }

      break;

    case UnitId::dark_archon:
      break;

    case UnitId::corsair:
      if (canCastSpellOrder(unit, TechId::DisruptionWeb, OrderId::CastDisruptionWeb)) {
        CUnit *target = findBestDisruptionWebTarget(unit, isUnitBeingAttacked);
        
        if (unit->mainOrderId == OrderId::CastDisruptionWeb
            && unit->orderTarget.unit == target)
          return false;

        if (aiCastSpellOrder(unit, target, OrderId::CastDisruptionWeb))
          return true;
      }

      break;
  }

  return false;
}

} //AI


//-------- Helper function definitions. Do NOT modify! --------//
namespace {

//Logically equivalent to function @ 0x004A11E0
bool canCastSpellOrder(const CUnit *unit, u8 techId, u8 orderId) {
  u16 energyCost = 0;
  if (Order::TechUsed[orderId] < TechId::None)
    energyCost = Tech::EnergyCost[Order::TechUsed[orderId]] * 256;

  if (unit->energy >= energyCost || scbw::isCheatEnabled(CheatFlags::TheGathering))
    return unit->canUseTech(techId, unit->playerId);

  return false;
}

//Logically equivalent to function @ 0x004A1290
bool aiCastSpellOrder(CUnit *unit, CUnit *target, u8 orderId, u8 aiActionFlag) {
  if (!target || target->aiActionFlags & aiActionFlag)
    return false;

  if (unit->mainOrderId != orderId) {
    unit->orderTo(orderId, target);
    target->aiActionFlags |= aiActionFlag;
  }
  return true;
}

} //unnamed namespace
