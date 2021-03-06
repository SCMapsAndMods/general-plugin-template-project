#include "definitions.h"
#include "Plugin.h"
#include "hook_tools.h"

//Hook header files
#include "hooks/harvest.h"

/// This function is called when the plugin is loaded into StarCraft.
/// You can enable/disable each group of hooks by commenting them.
/// You can also add custom modifications to StarCraft.exe by using:
///    memoryPatch(address_to_patch, value_to_patch_with);

BOOL WINAPI Plugin::InitializePlugin(IMPQDraftServer *lpMPQDraftServer) {
  //StarCraft.exe version check
  char exePath[300];
  const DWORD pathLen = GetModuleFileName(NULL, exePath, sizeof(exePath));
  if (pathLen == sizeof(exePath)) {
    MessageBox(NULL, "Error: Cannot check version of StarCraft.exe. The file path is too long.", NULL, MB_OK);
    return FALSE;
  }
  if (!checkStarCraftExeVersion(exePath))
    return FALSE;

  hooks::injectHarvestResource();

  return TRUE;
}
