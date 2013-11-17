#define _CRT_SECURE_NO_WARNINGS   //For strcpy()
#include "definitions.h"
#include "Plugin.h"
#include <cstring>

//Checks that the plugin's name is within the max length set by MPQDraft
C_ASSERT(sizeof(PLUGIN_NAME) <= MPQDRAFT_MAX_PLUGIN_NAME);

BOOL WINAPI Plugin::Identify(LPDWORD lpdwPluginID) {
  if (lpdwPluginID == NULL) {
    MessageBox(NULL, "Cannot assign plugin ID", NULL, MB_OK);
    return FALSE;
  }

  *lpdwPluginID = PLUGIN_ID;
  return TRUE;
}

BOOL WINAPI Plugin::GetPluginName(LPSTR lpszPluginName, DWORD nNameBufferLength) {
  if (lpszPluginName == NULL) {
    MessageBox(NULL, "Cannot assign plugin name", NULL, MB_OK);
    return FALSE;
  }

  if (nNameBufferLength < sizeof(PLUGIN_NAME)) {
    MessageBox(NULL, "Plugin name buffer is too short", NULL, MB_OK);
    return FALSE;
  }

  strcpy(lpszPluginName, PLUGIN_NAME);
  return TRUE;
}

BOOL WINAPI Plugin::CanPatchExecutable(LPCSTR lpszEXEFileName) {
  if (lpszEXEFileName == NULL) {
    MessageBox(NULL, "Invalid path to patch target executable", NULL, MB_OK);
    return FALSE;

  }

  //TODO: Add StarCraft.exe version check
  //For now, we assume the modder knows which version of StarCraft he/she is using.
  return TRUE;
}

//Gotta define this in a separate file
//BOOL WINAPI Plugin::Configure(HWND hParentWnd);

BOOL WINAPI Plugin::ReadyForPatch() {
  //By default, nothing to check
  return TRUE;
}

BOOL WINAPI Plugin::GetModules(MPQDRAFTPLUGINMODULE *lpPluginModules, LPDWORD lpnNumModules) {
  if (lpnNumModules == NULL) {
    MessageBox(NULL, "Cannot assign number of modules", NULL, MB_OK);
    return FALSE;
  }

  //Tell MPQDraft that this plugin uses 1 module file.
  *lpnNumModules = 1;

  if (lpnNumModules != NULL) {
    //Pass the plugin module information to MPQDraft
    lpPluginModules[0].dwComponentID = PLUGIN_ID;
    lpPluginModules[0].dwModuleID = SETTINGS_INI_ID;
  }

  return TRUE;
}

//Gotta define this in a separate file
//BOOL WINAPI Plugin::InitializePlugin(IMPQDraftServer *lpMPQDraftServer);

BOOL WINAPI Plugin::TerminatePlugin() {
  //By default, do nothing
  return TRUE;
}
