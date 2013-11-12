#include "weapon_cooldown.h"
#include "../SCBW/scbwdata.h"
#include "../SCBW/enumerations.h"
#include <algorithm>

namespace hooks {

/// Calculates the unit's weapon cooldown, factoring in upgrades and status effects.
///
/// @return		The modified cooldown value.
u32 getModifiedWeaponCooldownHook(const CUnit* unit, u8 weaponId) {
	u32 cooldown = Weapon::Cooldown[weaponId];

	if (unit->acidSporeCount) {
    //Use proper rounding to better simulate the 12.5%-per-spore
    cooldown += (unit->acidSporeCount * cooldown + 4) / 8;
	}

  //Ensnare always overrides Stim Packs and Adrenal Glands
  if (unit->ensnareTimer)
    cooldown += (cooldown + 2) / 4;     // +25%, rounded properly
  else if (unit->stimTimer != 0 || unit->status & UnitStatus::CooldownUpgrade)
    cooldown = (cooldown * 4 + 3) / 6;  // -66.7%, rounded properly

	return CLAMP(cooldown, 5u, 250u);
}

} //hooks
