# Introduction #

Hook modules are the core of GPTP: they allow modders to directly alter how StarCraft works. The following table lists basic information about the hook modules.

# Hook Module List #

## General Modding ##

| **Hook Module** | **Editable Source File** | **Description** |
|:----------------|:-------------------------|:----------------|
| Apply Upgrade Flags | <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/apply_upgrade_flags.cpp'>hooks/apply_upgrade_flags.cpp</a></tt> | Controls movement speed/attack speed upgrades |
| Building Morph | <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/building_morph.cpp'>hooks/building_morph.cpp</a></tt> | Control which buildings can morph into other buildings |
| Bunker | <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/bunker_hooks.cpp'>hooks/bunker_hooks.cpp</a></tt> | <ul><li>Determine which unit can attack from inside Bunkers</li> <li>Which sprite overlays are used for units attacking inside Bunkers</li></ul> |
| Cloak Nearby Units | <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/cloak_nearby_units.cpp'>hooks/cloak_nearby_units.cpp</a></tt> | Control Arbiters' AoE cloaking field |
| Cloak Tech | <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/cloak_tech.cpp'>hooks/cloak_tech.cpp</a></tt> | Determine which units can use the Cloaking Field / Personnel Cloaking tech |
| Consume | <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/consume.cpp'>hooks/consume.cpp</a></tt> | Control Consume target check and spell effects |
| Detector | <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/detector.cpp'>hooks/detector.cpp</a></tt> | Control detector requirement and detection range |
| Game Hooks | <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/game_hooks.cpp'>hooks/game_hooks.cpp</a></tt> | Generic hook functions for modding |
| Resource Harvest | <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/harvest.cpp'>hooks/harvest.cpp</a></tt> | <ul><li>Resource amount harvested / carried by workers</li> <li>Image ID used to represent resource chunks being carried</li></ul> |
| Psi Field | <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/psi_field.cpp'>hooks/psi_field.cpp</a></tt> | Controls which units can provide Psi Fields |
| Rally Point | <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/rally_point.cpp'>hooks/rally_point.cpp</a></tt> | Controls how rally points are set and used |
| Recharge Shields | <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/recharge_shields.cpp'>hooks/recharge_shields.cpp</a></tt> | <ul><li>Shield recharge requirements</li> <li>Shield recharge rate and energy cost</li> <br>
<tr><td> Spider Mine </td><td> <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/spider_mine.cpp'>hooks/spider_mine.cpp</a></tt> </td><td> <ul><li>Spider Mine targeting requirements</li> <li>Spider Mine initial burrow delay</li></ul> </td></tr>
<tr><td> Stim Packs </td><td> <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/stim_packs.cpp'>hooks/stim_packs.cpp</a></tt> </td><td> Stim Packs HP cost and effects </td></tr>
<tr><td> Tech Target Check </td><td> <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/tech_target_check.cpp'>hooks/tech_target_check.cpp</a></tt> </td><td> <ul><li>Target check for various tech spells</li> <li>Tech use error messages</li></ul> </td></tr>
<tr><td> Transfer Tech & Upgrades </td><td> <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/transfer_tech_upgrades.cpp'>hooks/transfer_tech_upgrades.cpp</a></tt> </td><td> Determine which tech and upgrades are transferred by Mind Control </td></tr>
<tr><td> Unit Speed </td><td> <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/unit_speed.cpp'>hooks/unit_speed.cpp</a></tt> </td><td> Controls unit speed, acceleration, and turn speed, factoring in upgrades and status effects </td></tr>
<tr><td> Unit Morph </td><td> <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/unit_morph.cpp'>hooks/unit_morph.cpp</a></tt> </td><td> Control which unit can morph into other units </td></tr>
<tr><td> Update Status Effects </td><td> <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/update_status_effects.cpp'>hooks/update_status_effects.cpp</a></tt> </td><td> <ul><li>Irradiate damage effect</li> <li>Other various status effects</li></ul> </td></tr>
<tr><td> Update Unit State </td><td> <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/update_unit_state.cpp'>hooks/update_unit_state.cpp</a></tt> </td><td> <ul><li>HP and shield regeneration</li> <li>Terran building burn-down rate</li> <li>Attack delay after unburrowing</li> <li>Unit energy regeneration rate</li> <li>Cloaking energy consumption rate</li> <li>Other unit timers</li></ul> </td></tr>
<tr><td> Weapon Cooldown </td><td> <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/weapon_cooldown.cpp'>hooks/weapon_cooldown.cpp</a></tt> </td><td> Controls weapon cooldown, factoring in upgrades and status effects </td></tr>
<tr><td> Weapon Damage </td><td> <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/weapon_damage.cpp'>hooks/weapon_damage.cpp</a></tt> </td><td> Control how weapon damage is applied to a unit. This modifies the behavior of <tt><a href='CUnit#damageWith.md'>CUnit::damageWith()</a></tt>. </td></tr>
<tr><td> Fire Weapon </td><td> <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/weapon_fire.cpp'>hooks/weapon_fire.cpp</a></tt> </td><td> Control how weapons are fired. This affects the behavior of <tt><a href='CUnit#fireWeapon.md'>CUnit::fireWeapon()</a></tt>. </td></tr></tbody></table>

<h2>Unit Stats ##

| **Hook Module** | **Editable Source File** | **Description** |
|:----------------|:-------------------------|:----------------|
| Armor Bonus | <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/unit_stats/armor_bonus.cpp'>hooks/unit_stats/armor_bonus.cpp</a></tt> | Controls unit armor upgrades |
| Maximum Energy | <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/unit_stats/max_energy.cpp'>hooks/unit_stats/max_energy.cpp</a></tt> | Sets the maximum energy of a unit. Affects <tt><a href='CUnit#getMaxEnergy.md'>CUnit::getMaxEnergy()</a></tt>. |
| Sight Range | <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/unit_stats/sight_range.cpp'>hooks/unit_stats/sight_range.cpp</a></tt> | Controls unit sight range |
| Weapon Range | <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/unit_stats/weapon_range.cpp'>hooks/unit_stats/weapon_range.cpp</a></tt> | <ul><li>Weapon range upgrades (affects <tt><a href='CUnit#getMaxWeaponRange.md'>CUnit::getMaxWeaponRange()</a></tt>)</li> <li>Seek range (AKA target acquisition range) upgrades</li></ul> |

## Game Interface ##

| **Hook Module** | **Editable Source File** | **Description** |
|:----------------|:-------------------------|:----------------|
| Unit Tooltip | <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/hooks/unit_tooltip.cpp'>hooks/unit_tooltip.cpp</a></tt> | Customize the console tooltip text for unit weapon, armor, and shields |