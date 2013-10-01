General Plugin Template Project v2.34

Created by A_of_s_t
Modified by RavenWolf and pastelmind
Special thanks to Heinermann, xAngelSpiritx and bajadulce
as well as many StarCraft modders whose name we have yet to add here.

WARNING: Most of the new functions have been untested. If there are any bugs,
please report them in the GPTP thread @ BroodWarAI.com:
  http://www.broodwarai.com/forums/viewtopic.php?f=5&t=923
Alternatively, visit http://gptp.googlecode.com/ and submit the issue.


== Instructions for Using GPTP ==

1. The core plugin functions (gameOn(), gameEnd(), nextFrame()) can be edited in
   hooks/game_hooks.cpp.

2. The hooks/ directory contains many hook functions that control various
   aspects of the StarCraft engine. These are disabled by default, but you can
   enable them by adding the hooking code in initialize.h. Most of the time, you
   will only have to interact with these two files.

3. As before, you can edit the plugin's name and ID in definitions.h.

4. configure.h now provides a simple popup when you click "Configure" in
   FireGraft or MPQDraft.

5. Hooks are organized by category, and each hook is contained in two files,
   named {hook type}.h and {hook type}.cpp. If you wish to make your plugin
   smaller, you can remove the header/source file pair from the project.


== Changes ==

For the full changelog, see http://code.google.com/p/gptp/wiki/Changelog

v2.34
 * Dropped official support for Code::Blocks. A tutorial on creating a custom
   project will soon be available.
 * The hook system has been overhauled to the new header-source-injector format.
 * The underlying MPQDraft plugin structure has been overhauled.
   * initialize.h has been renamed to initialize.cpp
   * configure.h has been renamed to configure.cpp
 * Graphics changes:
   * Drawing functions have been merged into one function per shape.
   * Drawing functions now take an extra parameter of graphics::CoordType type.
 * Several API functions have been moved to member functions of CUnit/CSprite.
 + New API functions: scbw::showErrorMessageWithSfx(), scbw::refreshButtonSet(),
   CUnit::getMaxWeaponRange(), CUnit::getMaxEnergy(), CUnit::getArmor(),
   CUnit::getArmorBonus(), CUnit::getSightRange()
 + New hooks
   + hooks/harvest.cpp: Change how much resources are harvested and returned per
     each worker trip.
 * Bug fixes
   * Psi field sprites become automatically invisible when a selected psi field
     provider stops providing power.
   * Shapes drawn on the map no longer "lag" when the viewport is scrolled
     across the map.
 * hooks::updatePsiFieldProvider() has been replaced by
   hooks::updatePsiFieldProviders().

v2.33
 + GPTP now supports drawing dots, lines, boxes, and text directly on the screen
   and the map. See graphics\graphics.h for details.
 + New hooks:
   + Psi field - make (most) units generate psi fields
   + Arbiter cloaking - configure which units are (not) cloaked by Arbiters
 * Unit finder functions have been moved to a separate file (unit_finder.h).
 * Unit finder functions now use a separate structure (UnitFinderResult) to
   store search results.
 * Some API functions have been moved to structure members.
 * Several structure members have been renamed/modified.
 * The memoryPatchX() functions in tools.h have been merged into a template
   function.
 * Bug fixes:
   * Fixed a critical bug in Unit::getDistanceToTarget().
   * Fixed small bugs in unit finder functions.

v2.32
 + GPTP now supports Visual C++ out of the box (again). Tested with VC++ Express
   2005 and the Platform SDK.
 + Added new hook for controlling which units can fire from inside bunkers.
 + New API functions: scbw::isAlliedTo(), scbw::isGamePaused().
 * GPTP now supports StarCraft: Brood War v1.16.1 only.
 * Bug fixes:
   * Fixed a bug that caused Ensnared units to attack faster.

v2.31
 * Fixed a critical bug with doWeaponDamageHook().

v2.3
 * Hooks have been modified:
   * Function definitions of hooks now reside in the hooks\ directory.
     To modify any hooks, edit the *.CPP files.
   * Plague damage hooks have been replaced by the more powerful
     updateUnitStatus() hook.
 * Fixed missing hooks for setting rally points
 * Changed how the VERSION macro works--see definitions.h for details.
 * Large enumerations are now spread between multiple header files. Don't worry;
   you can still include enumerations.h to use all of them.
 + Added new enumeration: ImageId (SCBW\enumerations\ImageId.h)
 * UNIT::healthPoints has been renamed to UNIT::hitPoints.
 + Other numerous changes that should make modding easier

v2.23
 * Turns out UNIT::healthPoints is a signed 32-bit integer. Fixed.
 * Fixed typos:
   * UNIT struct: statisTime -> stasisTime
   * UnitStatus enum: GoundedBuilding -> GroundedBuilding
 * The UNIT::unitId member has been renamed to UNIT::id.
 + Added new API functions:
    hasPathToPos(), hasPathToTarget(), isTargetWithinRange(), getDistanceFast(),
    removeUnit()
 + Added new hooks:
   + Control what actually happens when you set a rally point
   + Control upgrade effects for weapon range and sight range
   + Control which units can detect.

v2.221
 + Hotfixed a problem that caused rally hook to crash.

v2.22
 + JmpCallPatch() has been replaced by jmpPatch() and callPatch(), reflecting
   the latest set of functions used in BWAPI. See tools.h for a thorough
   documentation.
 * Many existing hooks have been rewritten to use jmpPatch() and callPatch().
   This reduces quite a few lines of unnecessary code.
 * Fixed (well, deleted) a potential bug in stimPacksHpCostToEax2().
 * Fixed a potential bug in energyRegenAmountHook() that could make a unit's
   energy overflow in some weird cases.
 + Added a new pointer constant, CONSUME_ENERGY_GAIN. You can use this pointer
   to access the energy gain amount set in FireGraft. consumeEnergyGainHook()
   has been modified to not destroy this value.

v2.21
 * Many enumerations were renamed. I promise I won't rename them again.
 + Added some new API functions and one hook function for controlling unit rally
   orders.
 + You can now loop through units and bullets using linked lists (using tables
   is still supported).
 * Some namespaces were renamed.
 * The UNIT structure has been slightly modified.
 * memoryPatch() has been split to memoryPatch1(), memoryPatch2(), and
   memoryPatch4() to avoid ambiguities.
 * sightLimitExtender should work properly now.

v2.2
 * Massively reorganized header and source files.
 + Added many missing DAT offsets, copied from BWAPI's Offsets.h
 + Added some SCBW's internal structures, also copied from BWAPI
 + Added many enumerations, also copied from BWAPI
 + Added several hook functions discovered by pastelmind (special thanks to
   Heinermann)
 + You can now generate custom plugin IDs with MakeID.hta. It supports random
   IDs as well as checksum-based IDs. Just double-click the MakeID.hta file to
   run the app.
