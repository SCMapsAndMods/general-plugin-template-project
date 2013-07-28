General Plugin Template Project v2.31

Created by A_of_s_t
Modified by RavenWolf and pastelmind
Special thanks to Heinermann, xAngelSpiritx and bajadulce
as well as many StarCraft modders whose name we have yet to add here.

WARNING: Most of the new functions have been untested. If there are any bugs,
please report them in the GPTP thread: http://www.broodwarai.com/forums/viewtopic.php?f=5&t=923


== Instructions for Using GPTP with Visual C++ ==

1. Create a new Win32 DLL project in Visual C++. Create an empty project or
   delete any headers and/or source files that VC++ generates for you.

2. Download and copy all of the GPTP files into your project folder.

3. Import all C++ header and source files (*.h and *.cpp) to your project. You
   don't need to import readme.txt, qdp.def or MakeID.hta; just make sure they
   are there.

4. In the left pane, right-click your project name to open the project's
   Build Options panel (or something similar).

5. In Compiler Options, enable the "Single-threaded Runtime Library" (/ML) or
   the "Multi-Threaded Runtime Library" (/MT) option, whichever is available.

6. In the Linker Options, set the output file to $(ProjectName).qdp and the
   module definition file to qdp.def.

7. Note: Unlike GPTP v2.0, you do NOT have to force the compiler to specifically
   build qdp.cpp. Just make sure every CPP file is compiled.

8. Now for the actual programming:

  a. The core plugin functions (gameOn(), gameEnd(), nextFrame()) can be edited
     in hooks/game_hooks.cpp.

  b. The hooks/ directory contains many hook functions that control numerous
     aspects of the StarCraft engine. These are disabled by default, but you can
     enable them by adding the hooking code in initialize.h. Most of the time,
     you will only have to interact with these two files.

  c. As before, you can edit the plugin's name and ID in definitions.h.

  d. configure.h now provides a simple popup when you click "Configure" in
     FireGraft or MPQDraft.

  e. Hooks are organized by category, and each hook is contained in two files,
     named {hook type}.h and {hook type}.cpp. If you wish to make your plugin
     smaller, you can remove the header/source file pair from the project.


== Changes ==

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
