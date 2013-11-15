#include "definitions.h"
#include "Plugin.h"
#include "hook_tools.h"

//Hook header files
#include "hooks/rally_point.h"

/// This function is called when the plugin is loaded into StarCraft.
/// You can enable/disable each group of hooks by commenting them.
/// You can also add custom modifications to StarCraft.exe by using:
///    memoryPatch(address_to_patch, value_to_patch_with);

BOOL WINAPI Plugin::InitializePlugin(IMPQDraftServer *lpMPQDraftServer) {
  hooks::injectRallyHooks();
  return TRUE;
}
