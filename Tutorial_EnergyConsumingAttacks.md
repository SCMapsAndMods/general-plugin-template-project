# Introduction #

This tutorial will show you how to make a unit (a Wraith in this case) spend 5 energy every time it attacks a ground unit. If the Wraith does not have enough energy, it will wait until it has enough energy. This affects ground attacks only, and Wraiths can freely attack air units without spending energy.

Note: This tutorial is roughly based on the "[Wraith with energy weapons](http://modcrafters.com/forum/viewtopic.php?f=17&t=94)" article, though it has several differences.

# The Idea #

All units keep two timers for checking ground and air weapon cooldown. Whenever a unit attacks another unit, the attacker's ground/air weapon cooldown timers are set to the cooldown value defined in `weapons.dat`. Then the timer is reduced by 1 every frame until it reaches 0. The unit cannot attack until its cooldown timer expires.

The trick here is:

  1. Prevent the Wraith's ground weapon cooldown timer from reaching 0 if it does not have enough energy. Since ground and air weapons use separate timers, we can prevent the Wraith from using its air-to-ground weapon while not affecting its air-to-air weapon.
  1. If the Wraith actually attacks a ground unit, reduce its energy by 5.

# The Code #

## Step 1. Controlling the Weapon Cooldown Timer ##

Weapon cooldown timers are decremented in <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/update_unit_state.cpp'>hooks/update_unit_state.cpp</a></tt>, at lines 82-83:

```
  if (unit->groundWeaponCooldown)
    unit->groundWeaponCooldown--;
```

Add a check to see whether the unit is a Wraith, whose energy amount is less than 5. Note that 1 energy in-game equals 256 energy, so 5 energy in-game is equal to 1280 energy.

```
  if (unit->groundWeaponCooldown)
    unit->groundWeaponCooldown--;

  //Check if the unit is a Wraith with less than 5 energy
  if (unit->id == UnitId::wraith
      && unit->energy < 1280)
  {
  }
```

Then check the wraith's ground weapon cooldown timer. If the cooldown is 0, we increase it by 1 to prevent it from attacking:

```
  if (unit->groundWeaponCooldown)
    unit->groundWeaponCooldown--;

  //Check if the unit is a Wraith with less than 5 energy
  if (unit->id == UnitId::wraith
      && unit->energy < 1280
      && unit->groundWeaponCooldown == 0)
  {
    unit->groundWeaponCooldown = 1;
  }
```

There! Wraiths will not attack ground targets unless they have at least 5 energy.

## Step 2. Reducing Energy upon Attacking ##

Every time a unit attacks with a ranged weapon, a `CBullet` object is created. This behavior is controlled in <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/weapon_fire.cpp'>hooks/weapon_fire.cpp</a></tt>. This is where the attack detection code will be placed, between lines 52 and 53:

```
  if (weapons_dat::FlingyId[weaponId] != 0)
    createBullet(weaponId, unit, x, y, unit->playerId, unit->currentDirection1);
}
```

Check if the unit is a Wraith:

```
  if (weapons_dat::FlingyId[weaponId] != 0)
    createBullet(weaponId, unit, x, y, unit->playerId, unit->currentDirection1);

  //Check if the unit is a Wraith
  if (unit->id == UnitId::Wraith) {
  }
}
```

If so, test whether the unit is targeting a ground unit:

```
  if (weapons_dat::FlingyId[weaponId] != 0)
    createBullet(weaponId, unit, x, y, unit->playerId, unit->currentDirection1);

  //Check if the unit is a Wraith
  if (unit->id == UnitId::Wraith) {
    CUnit *target = unit->orderTarget.unit;
    
    //Check if the target exists and is a ground unit
    if (target && !(target->status & UnitStatus::InAir))
  }
}
```

If the attack target is a ground unit, reduce the Wraith's energy amount by 5.

```
  if (weapons_dat::FlingyId[weaponId] != 0)
    createBullet(weaponId, unit, x, y, unit->playerId, unit->currentDirection1);

  //Check if the unit is a Wraith
  if (unit->id == UnitId::Wraith) {
    CUnit *target = unit->orderTarget.unit;
    
    //Check if the target exists and is a ground unit
    if (target && !(target->status & UnitStatus::InAir)) {
      unit->energy -= 1280;
    }
  }
}
```

Note that if, for some reason, the Wraith has less than 1280 energy, this code will create an integer overflow bug. Therefore, the Wraith's energy amount must be checked; if it is less than 1280, set it to 0:

```
  if (weapons_dat::FlingyId[weaponId] != 0)
    createBullet(weaponId, unit, x, y, unit->playerId, unit->currentDirection1);

  //Check if the unit is a Wraith
  if (unit->id == UnitId::Wraith) {
    CUnit *target = unit->orderTarget.unit;
    
    //Check if the target exists and is a ground unit
    if (target && !(target->status & UnitStatus::InAir)) {
      if (unit->energy >= 1280)
        unit->energy -= 1280;
      else
        unit->energy = 0;
    }
  }
}
```

...and we're done! The Wraith now uses its energy for ground attacks.