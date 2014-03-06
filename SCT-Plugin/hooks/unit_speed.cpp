//Contains hooks that control unit movement speed, acceleration, and turn speed.

#include "unit_speed.h"
#include "../SCBW/enumerations.h"
#include "../SCBW/scbwdata.h"
#include <algorithm>

namespace hooks {

/// Calculates the unit's modified movement speed, factoring in upgrades and status effects.
///
/// @return		The modified speed value.
u32 getModifiedUnitSpeedHook(const CUnit* unit, u32 baseSpeed) {
	u32 speed = baseSpeed;

  if (unit->stimTimer || unit->status & UnitStatus::SpeedUpgrade) {
    speed += speed / 2; // +50% speed
    
    //Apply maximum for Scouts and Vultures
    if (unit->id == UnitId::scout
      || unit->id == UnitId::Hero_Artanis
      || unit->id == UnitId::mojo
      || unit->id == UnitId::vulture
      || unit->id == UnitId::jim_raynor_vulture)
    {
      if (speed > 2133)
        speed = 2133;   //maximum 8.33
    }

    if (speed < 853)
      speed = 853;      //minimum 3.33
  }

  if (unit->ensnareTimer)
    speed -= speed / 4; // -25% speed

	return speed;
}

/// Calculates the unit's acceleration, factoring in upgrades and status effects.
///
/// @return		The modified acceleration value.
u32 getModifiedUnitAccelerationHook(const CUnit* unit) {
	//Default StarCraft behavior
	u32 acceleration = flingy_dat::Acceleration[units_dat::Graphic[unit->id]];
	int modifier = (unit->stimTimer ? 1 : 0) - (unit->ensnareTimer ? 1 : 0)
                 + (unit->status & UnitStatus::SpeedUpgrade ? 1 : 0);
	if (modifier > 0)
		acceleration <<= 1;
	else if (modifier < 0)
		acceleration -= acceleration >> 2;
	return acceleration;
}

/// Calculates the unit's turn speed, factoring in upgrades and status effects.
///
/// @return		The modified turning speed value.
u32 getModifiedUnitTurnSpeedHook(const CUnit* unit) {
	u32 turnSpeed = flingy_dat::TurnSpeed[units_dat::Graphic[unit->id]];
	int modifier = (unit->stimTimer ? 1 : 0) - (unit->ensnareTimer ? 1 : 0)
                 + (unit->status & UnitStatus::SpeedUpgrade ? 1 : 0);
	if (modifier > 0)
		turnSpeed <<= 1;
	else if (modifier < 0)
		turnSpeed -= turnSpeed >> 2;

  return std::min(turnSpeed, 127u); //To prevent jerky movement bug
}

} //hooks
