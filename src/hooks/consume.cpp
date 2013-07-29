#include "consume.h"
#include "max_unit_energy.h"
#include "../SCBW/api.h"
#include "../SCBW/scbwdata.h"
#include "../SCBW/enumerations.h"

/// Helper function: Sets death scores for [unit] owned by [player].
void incrementDeathScores(const CUnit *unit, u8 player);


/// Decides whether the given unit can be Consumed.
bool unitIsConsumable(const CUnit* unit, u8 consumingPlayer) {
	using Unit::BaseProperty;
	using Unit::GroupFlags;
	using UnitProperty::Building;

	//Default StarCraft behavior
	return !(BaseProperty[unit->id] & Building)   //Cannot consume buildings
         && unit->playerId == consumingPlayer	  //Must be owned by the current player
         && GroupFlags[unit->id].isZerg		      //Must be a Zerg unit
         && unit->id != UnitId::larva;					//Cannot consume larvae
}

/// Is called when a unit is consumed.
void onConsumeUnit(CUnit* caster, CUnit *target) {
  //Default StarCraft behavior
  incrementDeathScores(target, caster->playerId);
  target->remove();
  if (!(target->status & UnitStatus::IsHallucination)) {  //If not a hallucination
    u16 energy = caster->energy + 12800; //50 energy
    u16 maxEnergy = getUnitMaxEnergy(caster);
    if (energy > maxEnergy)
      energy = maxEnergy;
    caster->energy = energy;
  }
}


/**** Definitions of helper functions. Do not edit anything below this! ****/

void incrementDeathScores(const CUnit *unit, u8 player) {
  __asm {
    PUSHAD
    MOV EDI, unit
    MOVZX EDX, player
    CALL offsets::Util_IncrementDeathScores
    POPAD
  }
}
