#include <windows.h>
#include "definitions.h"
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

/*Called when the user hits the configure button*/

void Configuration(HWND parentWindow) {
  /*
   *  You can't really change the version from
   *  here with the current format.  However, you
   *  can check the version and probably set any
   *  globals.
   */

  MessageBox(
    parentWindow,
    PLUGIN_NAME " (ID: " STR(PLUGIN_ID) ")"
    "\nMade by pastelmind"
    "\nBuilt on " __DATE__ " " __TIME__
    "\n"
    "\n[Features]"
    "\n - Add your own text here"
    ,
    PLUGIN_NAME,
    MB_TASKMODAL
  );
}

