#include "consume.h"
#include "max_unit_energy.h"
#include "../SCBW/api.h"
#include "../SCBW/scbwdata.h"
#include "../SCBW/enumerations.h"
#include <algorithm>


/// Helper function: Sets death scores for [unit] owned by [player].
void incrementDeathScores(const CUnit *unit, u8 player);

namespace hooks {

/// Decides whether the given unit can be Consumed.
bool unitIsConsumable(const CUnit* unit, u8 consumingPlayer) {
	//Default StarCraft behavior

  //Cannot consume buildings
  if (Unit::BaseProperty[unit->id] & UnitProperty::Building)
    return false;
  
  //Must be owned by the current player
  if (unit->playerId != consumingPlayer)
    return false;

  if (Unit::GroupFlags[unit->id].isZerg   //Must be a Zerg unit
      && unit->id != UnitId::larva)			  //Cannot consume larvae
    return true;

  return false;
}

/// Is called when a unit is consumed.
void onConsumeUnit(CUnit* caster, CUnit *target) {
  //Default StarCraft behavior
  incrementDeathScores(target, caster->playerId);
  target->remove();
  if (!(target->status & UnitStatus::IsHallucination)) {  //If not a hallucination
    u16 energy = caster->energy + 12800; //50 energy
    caster->energy = std::min(energy, getUnitMaxEnergy(caster));
  }
}

} //hooks

//-------- Helper function definitions. Do not edit anything below! --------//

const u32 Func_IncrementDeathScores = 0x00488AF0;
void incrementDeathScores(const CUnit *unit, u8 player) {
  __asm {
    PUSHAD
    MOV EDI, unit
    MOVZX EDX, player
    CALL Func_IncrementDeathScores
    POPAD
  }
}
