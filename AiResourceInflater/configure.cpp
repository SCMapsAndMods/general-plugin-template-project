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
    "\nBuilt on " __DATE__ " " __TIME__
    "\n"
    "\nThis plugin reads AI resource harvest values from " SETTINGS_INI_FILENAME
    ", which was automatically incorporated into your mod when you added this plugin in MPQDraft or FireGraft."
    " Thus, you do not need to include a copy of " SETTINGS_INI_FILENAME " with your mod executable for distribution."
    "\nTo change the harvest amount settings, you need to edit " SETTINGS_INI_FILENAME
    ", then remove and add the plugin again."
    ,
    PLUGIN_NAME,
    MB_TASKMODAL
  );

  return TRUE;
}

