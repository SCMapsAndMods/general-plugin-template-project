#include "irradiate.h"
#include <SCBW/UnitFinder.h>
#include <SCBW/scbwdata.h>
#include <SCBW/enumerations.h>

namespace hooks {

//Applies Irradiate effects for @p unit (which is Irradiated)
void doIrradiateDamageHook(CUnit *irradiatedUnit) {
  //Default StarCraft behavior

  auto irradiateProc = [&irradiatedUnit] (CUnit *unit) {
    //Damage organic units only
    if (!(units_dat::BaseProperty[unit->id] & UnitProperty::Organic))
      return;

    //Don't damage buildings
    if (units_dat::BaseProperty[unit->id] & UnitProperty::Building)
      return;

    //Don't damage larvae, eggs, and lurker eggs
    if (unit->id == UnitId::larva || unit->id == UnitId::egg || unit->id == UnitId::lurker_egg)
      return;

    //Irradiate splash damage does not affect burrowed units
    if (unit != irradiatedUnit && unit->status & UnitStatus::Burrowed)
      return;

    //Check if the unit is within distance, or is inside the same transport
    if (irradiatedUnit->status & UnitStatus::InTransport
        || irradiatedUnit->getDistanceToTarget(unit) <= 32)
    {
      const s32 damage = weapons_dat::DamageAmount[WeaponId::Irradiate] * 256 / weapons_dat::Cooldown[WeaponId::Irradiate];
      unit->damageWith(damage, WeaponId::Irradiate, irradiatedUnit->irradiatedBy, irradiatedUnit->irradiatePlayerId);
    }
  };

  //No splash if burrowed
  if (irradiatedUnit->status & UnitStatus::Burrowed) {
    irradiateProc(irradiatedUnit);
  }
  //If inside a transport, damage all units loaded within
  else if (irradiatedUnit->status & UnitStatus::InTransport) {
    const CUnit *transport = irradiatedUnit->connectedUnit;
    if (transport != NULL) {
      for (int i = 0; i < units_dat::SpaceProvided[transport->id]; ++i) {
        CUnit *loadedUnit = transport->getLoadedUnit(i);
        if (loadedUnit)
          irradiateProc(loadedUnit);
      }
    }
  }
  //Find and iterate nearby units
  else {
    scbw::UnitFinder unitFinder(irradiatedUnit->getX() - 160,
                                irradiatedUnit->getY() - 160,
                                irradiatedUnit->getX() + 160,
                                irradiatedUnit->getY() + 160);
    unitFinder.forEach(irradiateProc);
  }
}

} //hooks