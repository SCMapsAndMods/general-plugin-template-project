Here are some tips for modders who have experience with GPTP v2.0 or v2.1.

### Where is game.cpp? ###

game.cpp has been moved to `hooks/game_hooks.cpp`, where it sits with many other hook functions. For cleanliness, many hooks that were in game.cpp have been moved to their own header and source files in the `hooks/` directory.

### So many files! How do I edit them? ###

You can edit any source files (`*.cpp`) in the `hooks/` directory for changing StarCraft's default behavior. You can also edit `definitions.h` to change your plugin's name and version, as well as `configure.h` to modify the message box that shows when you click "Configure" in FireGraft or MPQDraft. However, editing anything else may potentially break the game or plugin, so be careful! You can always read and include the header files (`*.h`) in the `SCBW/` directory, though.