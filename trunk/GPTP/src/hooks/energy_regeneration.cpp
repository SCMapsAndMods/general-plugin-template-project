#include "energy_regeneration.h"
#include "max_unit_energy.h"
#include "../SCBW/enumerations.h"
#include "../SCBW/scbwdata.h"
#include "../SCBW/api.h"

//Helper function definitions
static void setSecondaryOrder(CUnit *unit, u8 orderId);

/// Determines cloaking energy consumption.
u16 getCloakingEnergyConsumption(const CUnit *unit) {
  //Default StarCraft behavior
  if (unit->id == UnitId::TerranGhost
      || unit->id == UnitId::Hero_SarahKerrigan
      || unit->id == UnitId::Hero_InfestedKerrigan
      || unit->id == UnitId::Hero_SamirDuran
      || unit->id == UnitId::Hero_InfestedDuran
      || unit->id == UnitId::Hero_AlexeiStukov)
    return 10;
  else if (unit->id == UnitId::TerranWraith
           || unit->id == UnitId::Hero_TomKazansky
           )
    return 13;
  else
    return 0;
}

/// This hook handles energy regeneration, as well as energy drain for cloaking.
void regenerateEnergyHook(CUnit *unit) {
  //Default StarCraft behavior
  using scbw::isCheatEnabled;
  using CheatFlags::TheGathering;

  if (!(Unit::BaseProperty[unit->id] & UnitProperty::Spellcaster) //If the unit is not a spellcaster
      || unit->status & UnitStatus::IsHallucination               //...or is a hallucination
      || !(unit->status & UnitStatus::Completed)                  //...or is not fully constructed
      )
    return; //No need to regenerate energy

  //Spend energy for cloaked units
  if (unit->status & (UnitStatus::Cloaked | UnitStatus::RequiresDetection)  //If the unit is cloaked
      && !(unit->status & UnitStatus::CloakingForFree)                      //...and must consume energy to stay cloaked
      && !isCheatEnabled(TheGathering)                                      //...and the energy cheat is not available
      ) {
    u16 cloakingEnergyCost = getCloakingEnergyConsumption(unit);
    if (unit->energy < cloakingEnergyCost) {
      if (unit->secondaryOrderId == OrderId::Cloak)
        setSecondaryOrder(unit, OrderId::Nothing2); //Supposedly, immediately decloaks the unit.
      return;
    }
    unit->energy -= cloakingEnergyCost;
  }
  else {
    u16 maxEnergy;
    if (unit->id == UnitId::dark_archon
        && unit->mainOrderId == OrderId::CompletingArchonSummon
        && !(unit->mainOrderState)
        )
      maxEnergy = 12800;  //50 * 256; Identical to energy amount on spawn
    else
      maxEnergy = getUnitMaxEnergy(unit);

    if (unit->energy != maxEnergy) {
      u16 energy = unit->energy + 8;
      if (energy > maxEnergy)
        energy = maxEnergy;
      unit->energy = energy;
    }
  }

  //If the unit is currently selected, redraw its graphics
  if (unit->sprite->flags & 8) {
    for (CImage *i = unit->sprite->imageHead; i; i = i->link.next)
      if (i->paletteType == 11)
        i->flags |= 1;
  }
}


/**** Helper function definitions. Do not change anything below this! ****/

//Logically same as the function @ 0x004743D0
static void setSecondaryOrder(CUnit *unit, u8 orderId) {
  if (unit->secondaryOrderId == orderId)
    return;
  unit->secondaryOrderId = orderId;
  unit->_unknown_0x0E8 = 0;
  unit->_unknown_0x0EA = 0;
  unit->currentBuildUnit = 0;
  unit->secondaryOrderState = 0;
}
