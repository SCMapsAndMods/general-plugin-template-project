#include "definitions.h"
#include "Plugin.h"
#include "hook_tools.h"

//Hook header files
#include "hooks/game_hooks.h"
#include "hooks/bunker_hooks.h"
#include "hooks/cloak_nearby_units.h"
#include "hooks/detector.h"
#include "hooks/energy_regeneration.h"
#include "hooks/irradiate.h"
#include "hooks/max_unit_energy.h"
#include "hooks/recharge_shields.h"
#include "hooks/sight_range.h"
#include "hooks/spider_mine.h"
#include "hooks/stim_packs.h"
#include "hooks/tech_target_check.h"
#include "hooks/unit_speed.h"
#include "hooks/unit_tooltip.h"
#include "hooks/update_status_effects.h"
#include "hooks/update_unit_timers.h"
#include "hooks/weapon_cooldown.h"
#include "hooks/weapon_damage.h"
#include "hooks/weapon_range.h"
#include "hooks/unit_destructor_special.h"
#include "hooks/psi_field.h"
#include "hooks/unit_morph.h"
#include "hooks/building_morph.h"

#include "graphics/draw_hook.h"
#include "AI/spellcasting.h"

/// This function is called when the plugin is loaded into StarCraft.
/// You can enable/disable each group of hooks by commenting them.
/// You can also add custom modifications to StarCraft.exe by using:
///    memoryPatch(address_to_patch, value_to_patch_with);

BOOL WINAPI Plugin::InitializePlugin(IMPQDraftServer *lpMPQDraftServer) {
  hooks::injectGameHooks();

  hooks::injectBunkerHooks();
  
  hooks::injectCloakNearbyUnits();

  hooks::injectDetectorHooks();

  hooks::injectEnergyRegenerationHook();

  hooks::injectIrradiateHook();

  hooks::injectUnitMaxEnergyHook();

  hooks::injectRechargeShieldsHooks();

  hooks::injectSightRangeHook();
  hooks::injectSpiderMineHooks();
  hooks::injectStimPacksHooks();

  hooks::injectTechTargetCheckHooks();

  hooks::injectUnitSpeedHooks();
  hooks::injectUnitTooltipHook();
  hooks::injectUpdateStatusEffects();
  hooks::injectUpdateUnitTimers();

  hooks::injectWeaponCooldownHook();
  hooks::injectWeaponDamageHook();
  hooks::injectWeaponRangeHooks();

  hooks::injectUnitDestructorSpecial();
  hooks::injectPsiFieldHooks();

  hooks::injectUnitMorphHooks();
  hooks::injectBuildingMorphHooks();

  hooks::injectDrawHook();
  hooks::injectSpellcasterAI();

  return TRUE;
}
