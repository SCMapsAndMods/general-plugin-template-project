#include "spellcasting.h"

#include "spells/yamato_gun.h"
#include "spells/lockdown.h"
#include "spells/launch_nuke.h"
#include "spells/emp_shockwave.h"
#include "spells/defensive_matrix.h"
#include "spells/irradiate.h"
#include "spells/restoration.h"
#include "spells/optical_flare.h"

#include "spells/parasite.h"
#include "spells/spawn_broodlings.h"
#include "spells/ensnare.h"
#include "spells/plague.h"
#include "spells/dark_swarm.h"

#include "spells/psi_storm.h"
#include "spells/hallucination.h"
#include "spells/stasis_field.h"
#include "spells/recall.h"
#include "spells/feedback.h"
#include "spells/mind_control.h"
#include "spells/maelstrom.h"
#include "spells/disruption_web.h"

#include <algorithm>

//-------- Helper function declarations. Do NOT modify! --------//
namespace {

bool canCastSpellOrder(const CUnit *unit, u8 techId, u8 orderId);
bool aiCastSpellOrder(CUnit *unit, CUnit *target, u8 orderId, u8 aiActionFlag = 1);
u16 getOrderEnergyCost(u8 orderId);
bool isNukeTimerReady(s8 playerId);
CUnit* getLoadedSilo(CUnit *ghost);

} //unnamed namespace

namespace AI {

//Attempts make the @p unit cast a spell.
bool AI_spellcasterHook(CUnit *unit, bool isUnitBeingAttacked) {
  if (!isUnitBeingAttacked
      && AIScriptController[unit->playerId].spellcasterTimer != 0)
    return false;

  u16 energyReserve;

  switch (unit->id) {
    case UnitId::battlecruiser:
      //Yamato Gun
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
      //Lockdown
      if (canCastSpellOrder(unit, TechId::Lockdown, OrderId::MagnaPulse)) {
        CUnit *target = findBestLockdownTarget(unit, isUnitBeingAttacked);
        
        if (unit->mainOrderId == OrderId::MagnaPulse
            && unit->orderTarget.unit == target)
          return false;

        if (aiCastSpellOrder(unit, target, OrderId::MagnaPulse))
          return true;
      }

      //Launch Nuclear Missile
      if (isNukeTimerReady(unit->playerId)) {
        CUnit *silo = getLoadedSilo(unit);
        if (!silo) return false;

        CUnit *target = findBestNukeLaunchTarget(unit, isUnitBeingAttacked);

        if (aiCastSpellOrder(unit, target, OrderId::NukePaint)) {
          silo->building.silo.nuke->connectedUnit = unit;
          AIScriptController[unit->playerId].AI_LastNukeTime = *elapsedTime;
          return true;
        }
      }

      break;

    case UnitId::science_vessel:
      //EMP Shockwave
      if (canCastSpellOrder(unit, TechId::EMPShockwave, OrderId::EmpShockwave)) {
        CUnit *target = findBestEmpShockwaveTarget(unit, isUnitBeingAttacked);
        
        if (unit->mainOrderId == OrderId::EmpShockwave
            && unit->orderTarget.unit == target)
          return false;

        if (aiCastSpellOrder(unit, target, OrderId::EmpShockwave))
          return true;
      }

      //Defensive Matrix
      if (canCastSpellOrder(unit, TechId::DefensiveMatrix, OrderId::DefensiveMatrix)) {
        CUnit *target = findBestDefensiveMatrixTarget(unit, isUnitBeingAttacked);
        
        if (unit->mainOrderId == OrderId::DefensiveMatrix
            && unit->orderTarget.unit == target)
          return false;

        if (aiCastSpellOrder(unit, target, OrderId::DefensiveMatrix))
          return true;
      }

      //Irradiate
      if (canCastSpellOrder(unit, TechId::Irradiate, OrderId::Irradiate)) {
        CUnit *target = findBestIrradiateTarget(unit, isUnitBeingAttacked);
        
        if (unit->mainOrderId == OrderId::Irradiate
            && unit->orderTarget.unit == target)
          return false;

        if (aiCastSpellOrder(unit, target, OrderId::Irradiate))
          return true;
      }

      break;

    case UnitId::medic:
      //Restoration
      if (canCastSpellOrder(unit, TechId::Restoration, OrderId::Restoration)) {
        CUnit *target = findBestRestorationTarget(unit, isUnitBeingAttacked);
        
        if (unit->mainOrderId == OrderId::Restoration
            && unit->orderTarget.unit == target)
          return false;

        if (aiCastSpellOrder(unit, target, OrderId::Restoration))
          return true;

        if (unit == AIScriptController[unit->id].mainMedic
            && unit->mainOrderId != OrderId::Restoration)
        {
          CUnit *targetSituational = findBestRestorationTargetSituational(unit, isUnitBeingAttacked);
          if (aiCastSpellOrder(unit, targetSituational, OrderId::Restoration))
            return true;
        }
      }

      //Optical Flare
      if (unit->getMaxEnergy() == unit->energy
          && canCastSpellOrder(unit, TechId::OpticalFlare, OrderId::CastOpticalFlare)) {
        CUnit *target = findBestOpticalFlareTarget(unit, isUnitBeingAttacked);
        
        if (unit->mainOrderId == OrderId::CastOpticalFlare
            && unit->orderTarget.unit == target)
          return false;

        if (aiCastSpellOrder(unit, target, OrderId::CastOpticalFlare))
          return true;
      }

      break;

    case UnitId::queen:
      //Parasite
      if (isUmsMode(unit->playerId) && scbw::random() % 256 == 1
          || unit->getMaxEnergy() == unit->energy)
      {
        if (canCastSpellOrder(unit, TechId::Parasite, OrderId::CastParasite)) {
          CUnit *target = findBestParasiteTarget(unit, isUnitBeingAttacked);
          
          if (unit->mainOrderId == OrderId::CastParasite
              && unit->orderTarget.unit == target)
            return false;

          if (aiCastSpellOrder(unit, target, OrderId::CastParasite, 4))
            return true;

          return false;
        }
        if (isUmsMode(unit->playerId))
          return false;
      }

      //Spawn Broodlings
      if (canCastSpellOrder(unit, TechId::SpawnBroodlings, OrderId::SummonBroodlings)) {
        CUnit *target = findBestSpawnBroodlingsTarget(unit, isUnitBeingAttacked);
        
        if (unit->mainOrderId == OrderId::SummonBroodlings
            && unit->orderTarget.unit == target)
          return false;

        if (aiCastSpellOrder(unit, target, OrderId::SummonBroodlings)) {
          unit->order(OrderId::Move, unit->getX(), unit->getY(), NULL, UnitId::None, false);
          return true;
        }
      }

      //Ensnare
      if ((isUmsMode(unit->playerId) || unit->getMaxEnergy() == unit->energy)
          && canCastSpellOrder(unit, TechId::Ensnare, OrderId::Ensnare))
      {
        CUnit *target = findBestEnsnareTarget(unit, isUnitBeingAttacked);
        
        if (unit->mainOrderId == OrderId::Ensnare
            && unit->orderTarget.unit == target)
          return false;

        if (aiCastSpellOrder(unit, target, OrderId::Ensnare))
          return true;
      }

      break;

    case UnitId::defiler:
      //Plague
      if (canCastSpellOrder(unit, TechId::Plague, OrderId::Plague)) {
        CUnit *target = findBestPlagueTarget(unit, isUnitBeingAttacked);
        
        if (unit->mainOrderId == OrderId::Plague
            && unit->orderTarget.unit == target)
          return false;

        if (aiCastSpellOrder(unit, target, OrderId::Plague))
          return true;
      }

      //Dark Swarm
      if (canCastSpellOrder(unit, TechId::DarkSwarm, OrderId::DarkSwarm)) {
        CUnit *target = findBestDarkSwarmTarget(unit, isUnitBeingAttacked);
        
        if (unit->mainOrderId == OrderId::DarkSwarm
            && unit->orderTarget.unit == target)
          return false;

        if (aiCastSpellOrder(unit, target, OrderId::DarkSwarm))
          return true;
      }
      break;

    case UnitId::high_templar:
      //Psionic Storm
      if (canCastSpellOrder(unit, TechId::PsionicStorm, OrderId::PsiStorm)) {
        CUnit *target = findBestPsiStormTarget(unit, isUnitBeingAttacked);
        
        if (unit->mainOrderId == OrderId::PsiStorm
            && unit->orderTarget.unit == target)
          return false;

        if (aiCastSpellOrder(unit, target, OrderId::PsiStorm))
          return true;
      }

      //Hallucination
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
      //Stasis Field
      if (canCastSpellOrder(unit, TechId::StasisField, OrderId::StasisField)) {
        CUnit *target = findBestStasisFieldTarget(unit, isUnitBeingAttacked);
        
        if (unit->mainOrderId == OrderId::StasisField
            && unit->orderTarget.unit == target)
          return false;

        if (aiCastSpellOrder(unit, target, OrderId::StasisField))
          return true;
      }
      
      //Recall
      if (canCastSpellOrder(unit, TechId::Recall, OrderId::Teleport)) {
        CUnit *target = findBestRecallTarget(unit, isUnitBeingAttacked);
        
        if (unit->mainOrderId == OrderId::Teleport
            && unit->orderTarget.unit == target)
          return false;

        if (aiCastSpellOrder(unit, target, OrderId::Teleport))
          return true;
      }

      break;

    case UnitId::dark_archon:
      //Feedback
      if (canCastSpellOrder(unit, TechId::Feedback, OrderId::CastFeedback)) {
        CUnit *target = findBestFeedbackTarget(unit, isUnitBeingAttacked);
        
        if (unit->mainOrderId == OrderId::CastFeedback
            && unit->orderTarget.unit == target)
          return false;

        if (aiCastSpellOrder(unit, target, OrderId::CastFeedback))
          return true;
      }
      
      //Mind Control
      if (canCastSpellOrder(unit, TechId::MindControl, OrderId::CastMindControl)) {
        CUnit *target = findBestMindControlTarget(unit, isUnitBeingAttacked);
        
        if (unit->mainOrderId == OrderId::CastMindControl
            && unit->orderTarget.unit == target)
          return false;

        if (aiCastSpellOrder(unit, target, OrderId::CastMindControl))
          return true;
      }
      
      //Maelstrom
      energyReserve = getOrderEnergyCost(OrderId::CastMaelstrom)
                      + getOrderEnergyCost(OrderId::CastMindControl);
      energyReserve = std::min(energyReserve, unit->getMaxEnergy());

      if ((unit->energy >= energyReserve || scbw::isCheatEnabled(CheatFlags::TheGathering))
          && unit->canUseTech(TechId::Maelstorm, unit->playerId))
      {
        CUnit *target = findBestMaelstromTarget(unit, isUnitBeingAttacked);
        
        if (unit->mainOrderId == OrderId::CastMaelstrom
            && unit->orderTarget.unit == target)
          return false;

        if (aiCastSpellOrder(unit, target, OrderId::CastMaelstrom))
          return true;
      }

      break;

    case UnitId::corsair:
      //Disruption Web
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

//Logically equivalent to function @ 0x0049E1C0
u16 getOrderEnergyCost(u8 orderId) {
  if (Order::TechUsed[orderId] < TechId::None)
    return Tech::EnergyCost[Order::TechUsed[orderId]] * 256;
  else
    return 0;
}

//Logically equivalent to function @ 0x00446E50
bool isNukeTimerReady(s8 playerId) {
  return *elapsedTime >= AIScriptController[playerId].AI_LastNukeTime
                         + 60 * AIScriptController[playerId].AI_NukeRate;
}

//Based on function @ 0x00463360
CUnit* getLoadedSilo(CUnit *ghost) {
  for (CUnit *unit = firstPlayerUnit->unit[ghost->playerId];
       unit; unit = unit->playerNext)
  {
    if (unit->id == UnitId::nuclear_silo
        && unit->building.silo.hasNuke)
    {
      CUnit *nuke = unit->building.silo.nuke;
      if (!nuke->connectedUnit
          || nuke->connectedUnit == ghost
          || nuke->connectedUnit->id == UnitId::nuclear_silo)
        return unit;
    }
  }
  return NULL;
}

} //unnamed namespace
