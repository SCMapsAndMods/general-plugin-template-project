#include "recharge_shields.h"
#include "../SCBW/scbwdata.h"
#include "../SCBW/enumerations.h"
#include "../SCBW/api.h"

/// Decides whether the @p target can recharge shields from the @p battery.
bool unitCanRechargeShieldsHook(const CUnit *target, const CUnit *battery) {
  //Default StarCraft behavior
  using Unit::ShieldsEnabled;
  using Unit::GroupFlags;
  using Unit::MaxShieldPoints;

  //Check target conditions
  if (target->playerId != battery->playerId   //Is not owned by the player
      || !(ShieldsEnabled[target->id])  //Does not have shields
      || !(target->status & (UnitStatus::Completed | UnitStatus::IsBuilding)) //Is incomplete?
      || target->status & UnitStatus::GroundedBuilding)                       //Is a building
    return false;

  //Check target race
  const GroupFlag &ugf = GroupFlags[target->id];
  if (!ugf.isProtoss || (ugf.isTerran || ugf.isZerg)) //Is not a protoss unit
    return false;

  //Check target shield amount
  if (target->shields >= (MaxShieldPoints[target->id] << 8)) //Already has max shields
    return false;

  //Check battery conditions
  if (!(battery->status & UnitStatus::Completed)          //Is being warped in
      || !battery->energy                                 //Does not have energy
      || battery->status & UnitStatus::DoodadStatesThing  //Is unpowered
      || battery->lockdownTimer || battery->stasisTimer || battery->maelstromTimer //Is frozen
      )
    return false; //Cannot recharge from this battery

  if (target->pAI) { //Separate check for AI-controlled units
    if (target->mainOrderId == OrderId::RechargeShields1 || target->mainOrderId == OrderId::Pickup4)
      return false;
    if (target->orderQueueHead && target->orderQueueHead->orderId == OrderId::RechargeShields1)
      return false;
  }

  return true;
}

/// This hook is called every frame when a unit recharges shields.
void rechargeShieldsProcHook(CUnit *target, CUnit *battery) {
  //Default StarCraft behavior
  using scbw::isCheatEnabled;
  using CheatFlags::TheGathering;

  s32 shieldGain = 1280, energySpent = 640;
  const s32 maxShields = Unit::MaxShieldPoints[target->id] << 8;

   if (maxShields - target->shields < shieldGain) {
    shieldGain = maxShields - target->shields;
    energySpent = shieldGain / 2;
  }
  if (energySpent > battery->energy) {
    energySpent = battery->energy;
    shieldGain = energySpent * 2;
  }

  s32 shields = target->shields + shieldGain;
  if (shields > maxShields)
    shields = maxShields;
  target->shields = shields;

  if (!isCheatEnabled(TheGathering))
    battery->energy -= energySpent;
}

/// Determines when to stop the shield recharge process
bool canStopRechargeShieldsHook(const CUnit *target, const CUnit *battery) {
  //Default StarCraft behavior
  return target->shields >= (Unit::MaxShieldPoints[target->id] << 8)  //Shields are fully recharged
         || battery->energy == 0; //Shield Battery is drained
}

