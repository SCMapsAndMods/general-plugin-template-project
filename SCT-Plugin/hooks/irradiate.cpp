#include "irradiate.h"
#include "../SCBW/UnitFinder.h"
#include "../SCBW/scbwdata.h"
#include "../SCBW/enumerations.h"
#include "../SCBW/api.h"
#include <cstdio>

class IrradiateProc: public scbw::UnitFinderCallbackProcInterface {
  public:
    IrradiateProc(CUnit *irradiatedUnit) : irradiatedUnit(irradiatedUnit) {}
    void proc(CUnit *unit);

  private:
    CUnit *irradiatedUnit;
};

//Applied to each unit affected by Irradiate splash damage
void IrradiateProc::proc(CUnit *unit) {

  const u32 unitProps = Unit::BaseProperty[unit->id];

  //Don't damage buildings
  if (unitProps & UnitProperty::Building)
    return;

  //Don't damage larvae, eggs, lurker eggs, and Science Vessels
  if (unit->id == UnitId::larva
      || unit->id == UnitId::egg
      || unit->id == UnitId::lurker_egg
      || unit->id == UnitId::science_vessel)
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

  IrradiateProc irradiateProc(unit);

  //No splash if burrowed
  if (unit->status & UnitStatus::Burrowed) {
    irradiateProc.proc(unit);
  }
  //If inside a transport, damage all units loaded within
  else if (unit->status & UnitStatus::InTransport) {
    const CUnit *transport = unit->connectedUnit;
    if (transport != NULL) {
      for (int i = 0; i < Unit::SpaceProvided[transport->id]; ++i) {
        CUnit *loadedUnit = transport->getLoadedUnit(i);
        if (loadedUnit)
          irradiateProc.proc(loadedUnit);
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