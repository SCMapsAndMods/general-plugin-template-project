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
    "\n빌드 시점: " __DATE__ " " __TIME__
    "\n"
    "\n[기능]"
    "\n - 익스트랙터에서 매초마다 " STR(EXTRACTOR_MINERALS) " 미네랄과 " STR(EXTRACTOR_GAS) " 가스 제공"
    "\n   (+그래픽 효과)"
    ,
    PLUGIN_NAME,
    MB_TASKMODAL
  );

  return TRUE;
}

