#include "update_unit_timers.h"
#include "update_status_effects.h"
#include "energy_regeneration.h"
#include "../SCBW/scbwdata.h"
#include "../SCBW/enumerations.h"
#include "../SCBW/api.h"

namespace {
//Helper function: Returns true if the unit's HP <= 33%.
static bool unitHpIsInRedZone(const CUnit *unit);
} //unnamed namespace

namespace hooks {

/// Updates unit timers, regenerates hp and shields, and burns down Terran buildings.
/// Logically equivalent to function @ 0x004EC290
void updateUnitTimersHook(CUnit* unit) {
  //Default StarCraft logic

  //Timers
  if (unit->mainOrderTimer)
    unit->mainOrderTimer--;
  if (unit->groundWeaponCooldown)
    unit->groundWeaponCooldown--;
  if (unit->airWeaponCooldown)
    unit->airWeaponCooldown--;
  if (unit->spellCooldown)
    unit->spellCooldown--;

  //Shield regeneration
  if (units_dat::ShieldsEnabled[unit->id]) {
    s32 maxShields = (s32)(units_dat::MaxShieldPoints[unit->id]) << 8;
    if (unit->shields != maxShields) {
      unit->shields += 7;
      if (unit->shields > maxShields)
        unit->shields = maxShields;
      if (unit->sprite->flags & 8) {  //If the unit is currently selected, redraw its graphics
        for (CImage *i = unit->sprite->images.head; i; i = i->link.next)
          if (i->paletteType == 11)
            i->flags |= 1;
      }
    }
  }

  //Supposedly, allows Zerglings to attack after unburrowing without delay.
  if (unit->id == UnitId::zergling || unit->id == UnitId::Hero_DevouringOne)
    if (unit->groundWeaponCooldown == 0)
      unit->orderQueueTimer = 0;

  //Clear the healing flag every frame
  unit->isBeingHealed = 0;

  //Update unit status effects (stim, maelstrom, plague, etc.)
  if (unit->status & UnitStatus::Completed || !(unit->sprite->flags & 0x20)) {
    unit->cycleCounter++;
    if (unit->cycleCounter >= 8) {
      unit->cycleCounter = 0;
      //Call the native function in StarCraft.exe; Do NOT directly call the hook in update_status_effects.cpp
      updateStatusEffects(unit);
    }
  }

  //Only for fully-constructed units and buildings
  if (unit->status & UnitStatus::Completed) {
    //HP regeneration
    if (units_dat::BaseProperty[unit->id] & UnitProperty::RegeneratesHP
        && unit->hitPoints > 0
        && unit->hitPoints != units_dat::MaxHitPoints[unit->id]
        )
        unit->setHp(unit->hitPoints + 4);

    //Energy regeneration
    //Call the function in StarCraft.exe; do NOT directly call the hook function.
    regenerateEnergy(unit);

    //Recent order timer
    if (unit->recentOrderTimer)
      unit->recentOrderTimer--;

    //Self-destruct timer
    if (unit->removeTimer) {
      unit->removeTimer--;
      if (unit->removeTimer == 0) {
        unit->remove();
        return;
      }
    }

    //Terran building burn-down
    if (unit->getRace() == RaceId::Terran) {
      //Check if the unit is a grounded or lifted building
      if (unit->status & UnitStatus::GroundedBuilding
          || units_dat::BaseProperty[unit->id] & UnitProperty::FlyingBuilding)
      {
        //...whose current HP is less or equal to 33% of max HP
        if (unitHpIsInRedZone(unit)) {
          unit->damageHp(20, NULL, unit->lastAttackingPlayer);
        }
      }
    }
  }
}

} //hooks

namespace {

/**** Helper function definitions. Do not modify anything below this! ****/
const u32 Helper_UnitHpIsInRedZone  = 0x004022C0;
bool unitHpIsInRedZone(const CUnit *unit) {
  static Bool32 result;
  __asm {
    PUSHAD
    MOV ECX, unit
    CALL Helper_UnitHpIsInRedZone
    MOV result, EAX
    POPAD
  }
  return result != 0;
}

}
