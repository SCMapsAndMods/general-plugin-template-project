#include "definitions.h"
#include "Plugin.h"
#include "definitions.h"
#include "hook_tools.h"

//Hook header files
#include "hooks/harvest.h"

/// This function is called when the plugin is loaded into StarCraft.
/// You can enable/disable each group of hooks by commenting them.
/// You can also add custom modifications to StarCraft.exe by using:
///    memoryPatch(address_to_patch, value_to_patch_with);

BOOL WINAPI Plugin::InitializePlugin(IMPQDraftServer *lpMPQDraftServer) {
  if (lpMPQDraftServer == NULL) {
    MessageBox(NULL, "Cannot retrieve IMPQDraftServer", NULL, MB_OK);
    return FALSE;
  }

  char settings_ini_path[MPQDRAFT_MAX_PATH];
  BOOL success = lpMPQDraftServer->GetPluginModule(PLUGIN_ID, SETTINGS_INI_ID, settings_ini_path);
  if (!success) {
    MessageBox(NULL, "Cannot retrieve plugin module", NULL, MB_OK);
    return FALSE;
  }

  this->AI_mineralMined = GetPrivateProfileInt("Minerals", "AmountMined", 8, settings_ini_path);
  this->AI_mineralGain  = GetPrivateProfileInt("Minerals", "AmountGained", 8, settings_ini_path);
  this->AI_gasMined     = GetPrivateProfileInt("Gas", "AmountMined", 8, settings_ini_path);
  this->AI_gasGain      = GetPrivateProfileInt("Gas", "AmountGained", 8, settings_ini_path);
  this->AI_depletedGasGain = GetPrivateProfileInt("Gas", "DepletedAmountGained", 2, settings_ini_path);

  hooks::injectHarvestResource();

  return TRUE;
}
