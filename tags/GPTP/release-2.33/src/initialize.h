#pragma once
#include "definitions.h"
#include "SCBW/offsets.h"
#include "tools.h"
#include "hooks/game_hooks.h"
#include "hooks/apply_upgrade_flags.h"
#include "hooks/bunker_hooks.h"
#include "hooks/cloak_nearby_units.h"
#include "hooks/consume.h"
#include "hooks/detector_check.h"
#include "hooks/do_weapon_damage.h"
#include "hooks/energy_regeneration.h"
#include "hooks/max_unit_energy.h"
#include "hooks/rally_point.h"
#include "hooks/recharge_shields.h"
#include "hooks/sight_range.h"
#include "hooks/stim_packs.h"
#include "hooks/unit_armor_bonus.h"
#include "hooks/unit_speed.h"
#include "hooks/update_status_effects.h"
#include "hooks/update_unit_timers.h"
#include "hooks/weapon_cooldown.h"
#include "hooks/weapon_range.h"
#include "hooks/unit_destructor_special.h"
#include "hooks/psi_field.h"
#include "graphics/draw_hook.h"

/// This function is called when the plugin is loaded into StarCraft.
/// You can enable/disable each group of hooks by commenting them.
/// You can also add custom modifications to StarCraft.exe by using:
///    memoryPatch(address_to_patch, value_to_patch_with);

void Initialize() {
  jmpPatch(onGameStart,          offsets::GameStart);    // From BWAPI by Kovarex
  jmpPatch(onGameEnd,            offsets::GameEnd);      // From BWAPI by Kovarex
  jmpPatch(nextFrameHook,        offsets::NextLogicFrame);  // From BWAPI by Kovarex

  jmpPatch(applyUpgradeFlagsToNewUnitWrapper,
           offsets::Hook_ApplyUpgradeFlagsToNewUnit);
  jmpPatch(applyUpgradeFlagsToExistingUnitsWrapper,
           offsets::Hook_ApplyUpgradeFlagsToExistingUnits);

  jmpPatch(unitCanAttackInsideBunkerWrapper,
           offsets::Hook_UnitCanAttackInsideBunker);

  injectCloakNearbyUnitsHook();

  jmpPatch(unitIsConsumableWrapper,       offsets::Hook_UnitIsConsumable);
  jmpPatch(onConsumeUnitWrapper,          offsets::Hook_OnConsumeUnit);

  jmpPatch(unitCanDetectWrapper,          offsets::Hook_UnitCanDetect);

  jmpPatch(doWeaponDamageWrapper,         offsets::Func_DoWeaponDamage);

  jmpPatch(regenerateEnergyWrapper,       offsets::Hook_RegenerateEnergy);

  jmpPatch(getUnitMaxEnergyWrapper,       offsets::Hook_GetUnitMaxEnergy);

  jmpPatch(orderNewUnitToRallyWrapper,    offsets::Hook_OrderNewUnitToRally);
  callPatch(setRallyPositionWrapper_Call, offsets::Hook_SetRallyPosition_Call);
  jmpPatch(setRallyPositionWrapper_Jmp1,  offsets::Hook_SetRallyPosition_Jmp1);
  jmpPatch(setRallyPositionWrapper_Jmp2,  offsets::Hook_SetRallyPosition_Jmp2);
  callPatch(setRallyUnitWrapper_Call,     offsets::Hook_SetRallyUnit_Call);
  jmpPatch(setRallyUnitWrapper_Jmp1,      offsets::Hook_SetRallyUnit_Jmp1);
  jmpPatch(setRallyUnitWrapper_Jmp2,      offsets::Hook_SetRallyUnit_Jmp2);

  callPatch(rechargeShieldsProcWrapper,   offsets::Hook_RechargeShieldsProc);
  jmpPatch(unitCanRechargeShieldsWrapper, offsets::Hook_UnitCanRechargeShields);
  jmpPatch(canStopRechargeShieldsWrapper, offsets::Hook_CanStopRechargeShields);

  jmpPatch(getSightRangeWrapper,          offsets::Hook_GetSightRange);

  callPatch(useStimPacksAiWrapper,        offsets::Hook_UseStimPacksAi);
  jmpPatch(useStimPacksPlayerWrapper,     offsets::Hook_UseStimPacksPlayer);
  jmpPatch(canUseStimPacksWrapper,        offsets::Hook_CanUseStimPacks);

  jmpPatch(getArmorBonusWrapper,          offsets::Hook_GetArmorBonus);

  jmpPatch(getModifiedUnitSpeedWrapper,
           offsets::Hook_GetModifiedUnitSpeed);
  jmpPatch(getModifiedUnitAccelerationWrapper,
           offsets::Hook_GetModifiedUnitAcceleration);
  jmpPatch(getModifiedUnitTurnSpeedWrapper,
           offsets::Hook_GetModifiedUnitTurnSpeed);

  jmpPatch(updateStatusEffectsWrapper,    offsets::Hook_UpdateStatusEffects);

  jmpPatch(updateUnitTimersWrapper,       offsets::Hook_UpdateUnitTimers);

  jmpPatch(getModifiedWeaponCooldownWrapper,
           offsets::Hook_GetModifiedWeaponCooldown);

  jmpPatch(getSeekRangeWrapper,           offsets::Hook_GetSeekRange);
  jmpPatch(getMaxWeaponRangeWrapper,      offsets::Hook_GetMaxWeaponRange);

  hooks::unitDestructorSpecialInject();
  hooks::psiFieldHookInject();

  InjectDrawHook();
}
