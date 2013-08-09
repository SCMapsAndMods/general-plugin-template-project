#include "MPQDraftPlugin.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "configure.h"
#include "initialize.h"

const char* plugin_name = PLUGIN_NAME;

class MPQDraftPluginInterface : public IMPQDraftPlugin {
  HINSTANCE hInstance;
  public:
  BOOL WINAPI Identify(LPDWORD pluginID) {
    if (!pluginID) {
      mBox("Wtf? no space for pluginId?");
      return false;
    }
    *pluginID = PLUGIN_ID;
    return true;
  }
  BOOL WINAPI GetPluginName(LPSTR pPluginName,DWORD namebufferlength) {
    if (!pPluginName) {
      mBox("No buffer :O");
    }
    if (namebufferlength < strlen(plugin_name)) {
      mBox("Name buffer too short!");
      return false;
    }
    strcpy(pPluginName,plugin_name);
    return true;
  }
  BOOL WINAPI CanPatchExecutable(LPCSTR exefilename) {
	return TRUE;
  }
  BOOL WINAPI Configure(HWND parentwindow) {
    //Goes here when they hit Configure
	Configuration(parentwindow);
    return TRUE;
  }
  BOOL WINAPI ReadyForPatch() {
    //Right before it opens up SC, checks for panic?
    return TRUE;
  }
  BOOL WINAPI GetModules(MPQDRAFTPLUGINMODULE* pluginmodules,LPDWORD nummodules) {
    //Weird shit, i doubt you'll use it
    if (!nummodules) {
      return false;
      mBox("NumModules null?");
    }
    *nummodules = 0;
    return true;
  }
  BOOL WINAPI InitializePlugin(IMPQDraftServer* server) {
    //When Starcraft opens, this gets called in its own thread

	#ifdef DEBUG
		myfile.open("test.txt");
	#endif

	Initialize();
    return true;
  }
  BOOL WINAPI TerminatePlugin() {
    //Called when starcraft closes
    return true;
  }
  void WINAPI SetInstance(HINSTANCE hInst) {
    hInstance = hInst;
  }
};

MPQDraftPluginInterface thePluginInterface;

BOOL APIENTRY DllMain( HINSTANCE hInstance, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH:
        thePluginInterface.SetInstance(hInstance);
        break;

		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
        break;

		case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}

BOOL WINAPI GetMPQDraftPlugin(IMPQDraftPlugin **lppMPQDraftPlugin) {
  *lppMPQDraftPlugin = &thePluginInterface;
  return TRUE;
}
