Header file: <tt><a href='http://code.google.com/p/gptp/source/browse/trunk/GPTP/src/logger.h'>logger.h</a></tt>

# Introduction #

As of v2.41, GPTP provides a tool for writing text to a log file. This is useful when you want to debug your plugin, as conventional testing can be difficult with MPQDraft plugins.

# Basic Usage #

To use the logger, you must add the following line in the source file:
```
#include <logger.h>
```

Then on, you can write logs by using the `GPTP::logger` object, in a similar manner to using the standard output stream (`cout`):

```
GPTP::logger << "This is some log text" << std::endl;
GPTP::logger << unit->getName() << " at map position ("
             << unit->getX() << ", " << unit->getY() << ")\n";
```

When the plugin is loaded by StarCraft, the logger creates a text file with a `*.log` extension and writes all log to it. A new log file with a different name is created every time the mod is run.

The log file looks something like this:

```
Game started on Tue Feb 18 16:49:11 2014

Frame 0:
This is some log text
Terran Marine at map position (252, 793)

Frame 1:
This is some log text
Terran Marine at map position (254, 798)

...

Game ended on Tue Feb 18 17:07:31 2014
```

GPTP's logger automatically adds the current game frame number, as well as the time when the game was started/ended.

# Disabling the Logger #

Instead of erasing all logging statements to prepare for a release, you can simply disable the logger by building the plugin in Release mode. When the logger is disabled, all calls to `GPTP::logger` will be ignored, and the plugin will not create a log file when the game is run.

If you wish to disable logging while in Debug mode, you can erase the following statement in `logger.h` (line 7):

```
#define GPTP_LOGGING_ENABLED
```

# Technical Details #

`GPTP::logger` internally uses the C++ file output stream (`std::ofstream`) to write text to a log file.