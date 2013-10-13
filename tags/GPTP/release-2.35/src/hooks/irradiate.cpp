#include "irradiate.h"
#include "../SCBW/UnitFinder.h"
#include "../SCBW/scbwdata.h"
#include "../SCBW/enumerations.h"
#include "../SCBW/api.h"
#include <cstdio>

class IrradiateProc {
  public:
    IrradiateProc(CUnit *irradiatedUnit) : irradiatedUnit(irradiatedUnit) {}
    void operator ()(CUnit *unit) const;

  private:
    CUnit *irradiatedUnit;
};

//Applied to each unit affected by Irradiate splash damage
void IrradiateProc::operator ()(CUnit *unit) const {
  //Default StarCraft behavior

  const u32 unitProps = Unit::BaseProperty[unit->id];
  
  //Damage organic units only
  if (!(unitProps & UnitProperty::Organic))
    return;

  //Don't damage buildings
  if (unitProps & UnitProperty::Building)
    return;

  //Don't damage larvae, eggs, and lurker eggs
  if (unit->id == UnitId::larva || unit->id == UnitId::egg || unit->id == UnitId::ZergLurkerEgg)
    return;

  //Irradiate splash damage does not affect burrowed units
  if (unit != irradiatedUnit && unit->status & UnitStatus::Burrowed)
    return;

  //Distance check (if not inside a transport)
  if (irradiatedUnit->status & UnitStatus::InTransport
      || irradiatedUnit->getDistanceToTarget(unit) <= 32) {
    const s32 damage = Weapon::DamageAmount[WeaponId::Irradiate] * 256 / Weapon::Cooldown[WeaponId::Irradiate];
    unit->damageWith(damage,
                     WeaponId::Irradiate,
                     irradiatedUnit->irradiatedBy,
                     irradiatedUnit->irradiatePlayerId);
  }
}

namespace hooks {

//Applies Irradiate effects for @p unit (which is Irradiated)
void doIrradiateDamageHook(CUnit *unit) {
  //Default StarCraft behavior

  const IrradiateProc irradiateProc(unit);

  //No splash if burrowed
  if (unit->status & UnitStatus::Burrowed) {
    irradiateProc(unit);
  }
  //If inside a transport, damage all units loaded within
  else if (unit->status & UnitStatus::InTransport) {
    const CUnit *transport = unit->connectedUnit;
    if (transport != NULL) {
      for (int i = 0; i < Unit::SpaceProvided[transport->id]; ++i) {
        CUnit *loadedUnit = CUnit::getFromIndex(transport->loadedUnit[i].index);

        //Check if unit is valid
        if (loadedUnit == NULL)
          continue;

        //Check if unit actually has a sprite
        if (loadedUnit->sprite == NULL)
          continue;

        //Check if unit is not already dead
        if (loadedUnit->mainOrderId == OrderId::Die && loadedUnit->mainOrderState == 1)
          continue;

        //Check loaded unit ID
        if (loadedUnit->targetOrderSpecial != transport->loadedUnit[i].unitId)
          continue;

        irradiateProc(loadedUnit);
      }
    }
  }
  //Find and iterate nearby units
  else {
    scbw::UnitFinder unitFinder(unit->getX() - 160, unit->getY() - 160,
                                unit->getX() + 160, unit->getY() + 160);
    unitFinder.forEach(irradiateProc);
  }
}

} //hooks