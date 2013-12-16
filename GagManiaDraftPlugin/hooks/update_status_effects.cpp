#include "update_status_effects.h"
#include "../SCBW/api.h"
#include "../SCBW/enumerations.h"
#include "../SCBW/scbwdata.h"
#include "irradiate.h"

namespace {
//Helper functions that should be used only in this file
u8 getAcidSporeOverlayAdjustment(const CUnit* const unit);
} //unnamed namespace

namespace hooks {

//Detour for UpdateStatusEffects() (AKA RestoreAllUnitStats())
//Original function address: 0x00492F70 (SCBW 1.16.1)
//Note: this function is called every 8 ticks (when unit->cycleCounter reaches 8 == 0)
void updateStatusEffectsHook(CUnit *unit) {
  
  //벌처의 스파이더 마인 재충전 타이머 겸
  //드라군의 과부하 이후 마비 타이머 겸
  //각종 건물의 시간 증폭 타이머
  if (unit->unusedTimer) {
    unit->unusedTimer--;
    if (unit->unusedTimer == 0) {
      //벌처의 스파이더 마인 재충전
      if (unit->id == UnitId::vulture)
        unit->vulture.spiderMineCount++;

      //드라군의 마비가 풀림 => 일정 시간 동안 과부하 스킬 사용 못함
      else if (unit->id == UnitId::dragoon) {
        unit->status &= ~(UnitStatus::Disabled | UnitStatus::CanNotReceiveOrders);  //마비 해제
        unit->sprite->playerId = unit->playerId;  //플레이어 컬러 원상복귀
        unit->spellCooldown = 200;  //과부하 쿨다운
      }
    }
  }

  //스파이더 마인 재충전 시작 (하나 이상 남아있을 때 최대 5개까지 충전)
  if (unit->unusedTimer == 0 && unit->id == UnitId::vulture
      && 0 < unit->vulture.spiderMineCount && unit->vulture.spiderMineCount < 5)
  {
    unit->unusedTimer = 30; //충전 시간
  }

  if (unit->stasisTimer) {
    unit->stasisTimer--;
    if (unit->stasisTimer == 0)
      unit->removeStasisField();
  }

  if (unit->stimTimer) {
    unit->stimTimer--;
    if (unit->stimTimer == 0)
      unit->updateSpeed();
    
    //드라군의 과부하 지속 및 종료 단계
    if (unit->id == UnitId::dragoon) {
      //플레이어 컬러에서 검정색으로 빠르게 깜박이는 효과
      if (unit->stimTimer) {
		  if(unit->sprite->mainGraphic->animation==IscriptAnimation::WalkingToIdle)unit->playIscriptAnim(IscriptAnimation::Unused1);
        if (unit->stimTimer % 2)
          unit->sprite->playerId = unit->playerId;
        else
          unit->sprite->playerId = 160; //색깔을 검정색으로 바꿈
      }
      else {
        //과부하 종료 => 일정 시간 마비
        unit->orderToIdle();
        unit->unusedTimer = 30; //마비 시간
        unit->status |= UnitStatus::Disabled;
        unit->removeOverlay(IMAGE_DRAGOON_OVERLOAD_EFFECT); //과부하 그래픽 효과 제거
        unit->playIscriptAnim(IscriptAnimation::Unused2);
        unit->sprite->playerId = 160; //플레이어 컬러를 검정색으로 고정
      }
    }
  }

  if (unit->ensnareTimer) {
    unit->ensnareTimer--;
    if (unit->ensnareTimer == 0) {
      unit->removeOverlay(ImageId::EnsnareOverlay_Small, ImageId::EnsnareOverlay_Large);
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
    doIrradiateDamage(unit);
    if (unit->irradiateTimer == 0) {
      unit->removeOverlay(ImageId::Irradiate_Small, ImageId::Irradiate_Large);
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
      unit->removeOverlay(ImageId::PlagueOverlay_Small, ImageId::PlagueOverlay_Large);
  }

  if (unit->isUnderStorm)
    unit->isUnderStorm--;

  u8 previousAcidSporeCount = unit->acidSporeCount;
  for (int i = 0; i <= 8; ++i) {
    if (unit->acidSporeTime[i]) {
      unit->acidSporeTime[i]--;
      if (unit->acidSporeTime[i] == 0)
        unit->acidSporeCount--;
    }
  }
  if (unit->acidSporeCount) {
    u32 acidOverlayId = getAcidSporeOverlayAdjustment(unit) + ImageId::AcidSpores_1_Overlay_Small;
    if (!unit->getOverlay(acidOverlayId)) {
      unit->removeOverlay(ImageId::AcidSpores_1_Overlay_Small, ImageId::AcidSpores_6_9_Overlay_Large);
      if (unit->subunit)
        unit = unit->subunit;
      unit->sprite->createTopOverlay(acidOverlayId);
    }
  }
  else if (previousAcidSporeCount) {
    unit->removeOverlay(ImageId::AcidSpores_1_Overlay_Small, ImageId::AcidSpores_6_9_Overlay_Large);
  }
}

} //hooks

namespace {
/**** Helper function definitions. Do not change anything below this! ****/

u8 getAcidSporeOverlayAdjustment(const CUnit* const unit) {
  u8 adjustment = unit->acidSporeCount >> 1;
  return (adjustment < 3 ? adjustment : 3)
          + 4 * scbw::getUnitOverlayAdjustment(unit);
}

} //unnamed namespace
