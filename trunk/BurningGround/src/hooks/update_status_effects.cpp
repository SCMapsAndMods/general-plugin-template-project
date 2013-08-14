#include "update_status_effects.h"
#include "../SCBW/api.h"
#include "../SCBW/enumerations.h"
#include "../SCBW/scbwdata.h"


//Helper functions that should be used only in this file
static void runIrradiateDamageLoop(CUnit *unit);
static void reduceDefensiveMatrixHp(CUnit *unit, const s32 amount);
static u8 getAcidSporeOverlayAdjustment(const CUnit* const unit);


//Detour for UpdateStatusEffects() (AKA RestoreAllUnitStats())
//Original function address: 0x00492F70 (SCBW 1.16.1)
//Note: this function is called every 8 ticks (when unit->cycleCounter reaches 8 == 0)
void updateStatusEffectsHook(CUnit *unit) {
  //For several different uses in Burning Ground
  if (unit->unusedTimer)
    unit->unusedTimer--;

  if (unit->stasisTimer) {
    unit->stasisTimer--;
    if (unit->stasisTimer == 0)
      unit->removeStasisField();
  }

  if (unit->stimTimer) {
    unit->stimTimer--;
    if (unit->stimTimer == 0)
      unit->updateSpeed();
  }

  if (unit->ensnareTimer) {
    unit->ensnareTimer--;
    if (unit->ensnareTimer == 0) {
      scbw::removeOverlays(unit, ImageId::EnsnareOverlay_Small, ImageId::EnsnareOverlay_Large);
      unit->updateSpeed();
    }
  }

  if (unit->defensiveMatrixTimer) {
    unit->defensiveMatrixTimer--;
    if (unit->defensiveMatrixTimer == 0) {
      unit->reduceDefensiveMatrixHp(unit->defensiveMatrixHp);
    }
  }

  if (unit->irradiateTimer) {
    unit->irradiateTimer--;
    runIrradiateDamageLoop(unit);
    if (unit->irradiateTimer == 0) {
      scbw::removeOverlays(unit, ImageId::Irradiate_Small, ImageId::Irradiate_Large);
      unit->irradiatedBy = NULL;
      unit->irradiatePlayerId = 8;
    }
  }

  if (unit->lockdownTimer) {
    unit->lockdownTimer--;
    if (unit->lockdownTimer == 0)
      unit->removeLockdown();
  }

  if (unit->maelstromTimer) {
    unit->maelstromTimer--;
    if (unit->maelstromTimer == 0)
      unit->removeMaelstrom();
  }

  if (unit->plagueTimer) {
    unit->plagueTimer--;
    if (!(unit->status & UnitStatus::Invincible)) {
      s32 damage = (Weapon::DamageAmount[WeaponId::Plague] << 8) / 76;
      if (unit->hitPoints > damage)
        unit->damageHp(damage);
    }
    if (unit->plagueTimer == 0)
      scbw::removeOverlays(unit, ImageId::PlagueOverlay_Small, ImageId::PlagueOverlay_Large);
  }

  if (unit->isUnderStorm)
    unit->isUnderStorm--;

  for (int i = 0; i <= 8; ++i) {
    if (unit->acidSporeTime[i]) {
      unit->acidSporeTime[i]--;
      if (unit->acidSporeTime[i] == 0)
        unit->acidSporeCount--;
    }
  }
  if (unit->acidSporeCount) {
    u32 acidOverlayId = getAcidSporeOverlayAdjustment(unit) + ImageId::AcidSpores_1_Overlay_Small;
    if (!scbw::hasOverlay(unit, acidOverlayId)) {
      scbw::removeOverlays(unit, ImageId::AcidSpores_1_Overlay_Small, ImageId::AcidSpores_6_9_Overlay_Large);
      if (unit->subunit)
        unit = unit->subunit;
      scbw::createOverlay(unit->sprite, acidOverlayId);
    }
  }
  else if (unit->acidSporeCount) {
    scbw::removeOverlays(unit, ImageId::AcidSpores_1_Overlay_Small, ImageId::AcidSpores_6_9_Overlay_Large);
  }
}


/**** Helper function definitions. Do not change anything below this! ****/

namespace offsets {
const u32 IrradiateDamageLoop = 0x004555C0;
}

static void runIrradiateDamageLoop(CUnit *unit) {
  __asm {
    PUSHAD
    MOV ECX, unit
    CALL offsets::IrradiateDamageLoop
    POPAD
  }
}

//Logic copied from function @ 0x00454ED0
static void reduceDefensiveMatrixHp(CUnit *unit, const s32 amount) {
  if (unit->defensiveMatrixHp > amount) {
    unit->defensiveMatrixHp -= amount;
  }
  else {
    unit->defensiveMatrixHp = 0;
    unit->defensiveMatrixTimer = 0;
    scbw::removeOverlays(unit, ImageId::DefensiveMatrixFront_Small, ImageId::DefensiveMatrixFront_Large);
    scbw::removeOverlays(unit, ImageId::DefensiveMatrixBack_Small, ImageId::DefensiveMatrixBack_Large);
  }
  if (unit->defensiveMatrixTimer && !(unit->status & UnitStatus::Burrowed)) {
    if (unit->subunit)
      unit = unit->subunit;
    scbw::createOverlay(unit->sprite,
                        scbw::getUnitOverlayAdjustment(unit) + ImageId::DefensiveMatrixHit_Small);
  }
}

static u8 getAcidSporeOverlayAdjustment(const CUnit* const unit) {
  u8 adjustment = unit->acidSporeCount >> 1;
  return (adjustment < 3 ? adjustment : 3)
          + 4 * scbw::getUnitOverlayAdjustment(unit);
}
