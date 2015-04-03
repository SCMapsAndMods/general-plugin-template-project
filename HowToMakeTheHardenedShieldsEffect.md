# How to Make the Hardened Shields Effect #

In StarCraft 2, the [Immortal](http://us.battle.net/sc2/en/game/unit/immortal) is a Protoss unit with a passive ability called "Hardened Shields". This passive reduces all incoming damage to 10, as long as the Immortal has plasma shields. This tutorial will explain how to implement the same effect with GPTP.

## Files edited ##
`hooks\do_weapon_damage.cpp`

## Details ##

The `doWeaponDamageHook()` function in `hooks\do_weapon_damage.cpp` controls how weapon damage is actually applied when a unit attacks another. We are going to edit it so that when a unit is damaged, the hook function will check for the unit's ID. If it matches the unit type, then the damage amount would be reduced to 10. In this tutorial we will use the Protoss Reaver in place of the Immortal.

Open `do_weapon_damage.cpp` and change the following lines...

```
//...snip...
const u8 damageType = Weapon::DamageType[weaponId];

//Reduce Plasma Shields...but not just yet
s32 shieldReduceAmount = 0;
if (Unit::ShieldsEnabled[target->id] && target->shields >= 256) {
  if (damageType != DamageType::IgnoreArmor) {
    s32 plasmaShieldUpg = scbw::getUpgradeLevel(target->playerId, UpgradeId::ProtossPlasmaShields) << 8;
//...snip...
```

...like this:
```
//...snip...
const u8 damageType = Weapon::DamageType[weaponId];

bool isHardenedShieldsActivated = false;

//Reduce Plasma Shields...but not just yet
s32 shieldReduceAmount = 0;
if (Unit::ShieldsEnabled[target->id] && target->shields >= 256) {
  if (damageType != DamageType::IgnoreArmor) {
    //Hardened Shields check
    if (target->id == UnitId::reaver) {
      if (damage > 2560) {
        damage = 2560;
        isHardenedShieldsActivated = true;
      }
    }
    s32 plasmaShieldUpg = scbw::getUpgradeLevel(target->playerId, UpgradeId::ProtossPlasmaShields) << 8;
//...snip...
```

We just added:
  1. A boolean variable `isHardenedShieldsActivated` that holds whether Hardened Shields has been triggered. This will be explained shortly.
  1. An if-statement that checks if the unit is a Protoss Reaver. This is placed _inside_ the damage type check in order to prevent "Ignore Armor" attacks (Irradiate, Psi Storm, Feedback, etc.) from being reduced to 10 damage.
  1. Another if-statement that checks and reduces the damage amount to 10. Since 1 damage is equal to 256 inside StarCraft, we use the number 2560.

This deals with the actual Hardened Shields mechanic. But what about the fancy effect when the passive activates? This is where `isHardenedShieldsActivated` comes in.

Find and change the following lines...

```
    createShieldOverlay(target, direction);
}

//Update unit strength data (?)
target->airStrength = getUnitStrength(target, false);
```

...to this:

```
    createShieldOverlay(target, direction);
}

//Graphics effect, scavenge Unused Heal GRP
if (isHardenedShieldsActivated) {
  scbw::createOverlay(target->sprite, 971); //Unused Heal (Medium) - TMeHealM.grp
}

//Update unit strength data (?)
target->airStrength = getUnitStrength(target, false);
```

Here, we use the "Unused Heal (Medium)" entry (ID 971) in `images.dat`, as this shows a nice flickering shield-ish effect. Since the effect should be shown only when the damage is reduced, the boolean variable `isHardenedShieldsActivated` is checked here.

Done! Now build the project and add the plugin with FireGraft or MPQDraft.

## Result ##

![http://gptp.googlecode.com/svn/wiki/HardenedShieldsReaver.png](http://gptp.googlecode.com/svn/wiki/HardenedShieldsReaver.png)