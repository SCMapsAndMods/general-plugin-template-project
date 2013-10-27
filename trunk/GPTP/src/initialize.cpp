#include "definitions.h"
#include "Plugin.h"
#include "hook_tools.h"

//Hook header files
#include "hooks/game_hooks.h"
#include "hooks/apply_upgrade_flags.h"
#include "hooks/armor_bonus.h"
#include "hooks/bunker_hooks.h"
#include "hooks/cloak_nearby_units.h"
#include "hooks/cloak_tech.h"
#include "hooks/consume.h"
#include "hooks/detector.h"
#include "hooks/energy_regeneration.h"
#include "hooks/harvest.h"
#include "hooks/irradiate.h"
#include "hooks/max_unit_energy.h"
#include "hooks/rally_point.h"
#include "hooks/recharge_shields.h"
#include "hooks/sight_range.h"
#include "hooks/stim_packs.h"
#include "hooks/unit_speed.h"
#include "hooks/unit_tooltip.h"
#include "hooks/update_status_effects.h"
#include "hooks/update_unit_timers.h"
#include "hooks/weapon_cooldown.h"
#include "hooks/weapon_damage.h"
#include "hooks/weapon_range.h"
#include "hooks/unit_destructor_special.h"
#include "hooks/psi_field.h"

#include "graphics/draw_hook.h"
#include "AI/spellcasting.h"

/// This function is called when the plugin is loaded into StarCraft.
/// You can enable/disable each group of hooks by commenting them.
/// You can also add custom modifications to StarCraft.exe by using:
///    memoryPatch(address_to_patch, value_to_patch_with);

BOOL WINAPI Plugin::InitializePlugin(IMPQDraftServer *lpMPQDraftServer) {
  hooks::injectGameHooks();
  hooks::injectApplyUpgradeFlags();

  hooks::injectArmorBonusHook();

  hooks::injectBunkerHooks();
  
  hooks::injectCloakNearbyUnits();
  hooks::injectCloakingTechHooks();

  hooks::injectConsumeHooks();

  hooks::injectDamageWithHook();
  hooks::injectDetectorHooks();

  hooks::injectEnergyRegenerationHook();

  hooks::injectHarvestResource();

  hooks::injectIrradiateHook();

  hooks::injectUnitMaxEnergyHook();

  hooks::injectRallyHooks();
  hooks::injectRechargeShieldsHooks();

  hooks::injectSightRangeHook();
  hooks::injectStimPacksHooks();

  hooks::injectUnitSpeedHooks();

  hooks::injectUnitTooltipHook();

  hooks::injectUpdateStatusEffects();
  hooks::injectUpdateUnitTimers();

  hooks::injectWeaponCooldownHook();  
  hooks::injectWeaponRangeHooks();

  hooks::injectUnitDestructorSpecial();
  hooks::injectPsiFieldHooks();

  hooks::injectDrawHook();
  hooks::injectSpellcasterAI();

  return TRUE;
}
