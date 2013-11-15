#include "definitions.h"
#include "Plugin.h"
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

//Called when the user hits the configure button in MPQDraft or FireGraft
BOOL WINAPI Plugin::Configure(HWND hParentWnd) {
  /*
   *  You can't really change the version from
   *  here with the current format.  However, you
   *  can check the version and probably set any
   *  globals.
   */

  MessageBox(
    hParentWnd,
    PLUGIN_NAME " (ID: " STR(PLUGIN_ID) ")"
    "\nMade by pastelmind"
    "\nSpecial thanks to RavenWolf"
    "\nBuilt on " __DATE__ " " __TIME__
    "\nCreated with GPTP v2.37"
    "\n"
    "\n[Features]"
    "\n- Workers auto-harvest when rallied to minerals or gas."
    "\n  - Note: Workers and units use separate rally points."
    "\n- Units auto-enter transports and bunkers when rallied."
    ,
    PLUGIN_NAME,
    MB_TASKMODAL
  );

  return TRUE;
}

