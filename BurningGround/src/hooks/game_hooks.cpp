/// This is where the magic happens; program your plug-in's core behavior here.

#include "game_hooks.h"
#include "../SCBW/api.h"
#include "../SCBW/scbwdata.h"
#include "../SCBW/enumerations.h"
#include "../SCBW/ExtendSightLimit.h"
#include "../SCBW/utilities.h"
#include <cstdio>

bool firstRun = true;

/// This hook is called every frame; most of your plugin's logic goes here.
bool nextFrame() {
  if (!scbw::isGamePaused()) { //If the game is not paused

    if (firstRun) {
      firstRun = false;
    }

    // Loop through the unit table.
    // Warning: There is no guarantee that the current unit is actually a unit
    // rather than an unused space in memory.
    //for (int i = 0; i < UNIT_ARRAY_LENGTH; ++i) {
    //  CUnit* unit = &unitTable[i];
    //  //Write your code here
    //}

    // Alternative looping method
    // Guarantees that [unit] points to an actual unit.
    for (CUnit *unit = *firstVisibleUnit; unit; unit = unit->next) {
      //Write your code here

      //노라드 II와 발키리를 락다운, 스테이시스, 옵티컬, 인스네어, 이레디, 플레이그에 대해 무적으로 만듦
      //hooks/update_status_effects.cpp를 참고
      if (unit->id == UnitId::norad_ii || unit->id == UnitId::valkyrie) {
        if (unit->lockdownTimer) unit->removeLockdown();
        if (unit->stasisTimer) unit->removeStasisField();
        if (unit->isBlind) unit->isBlind = 0;

        //인스네어 제거
        if (unit->ensnareTimer) {
          unit->ensnareTimer = 0;
          scbw::removeOverlays(unit, ImageId::EnsnareOverlay_Small, ImageId::EnsnareOverlay_Large);
          unit->updateSpeed();
        }

        //이레디에이트 제거
        if (unit->irradiateTimer) {
          unit->irradiateTimer = 0;
          scbw::removeOverlays(unit, ImageId::Irradiate_Small, ImageId::Irradiate_Large);
          unit->irradiatedBy = NULL;
          unit->irradiatePlayerId = 8;
        }

        //플레이그 제거
        if (unit->plagueTimer) {
          unit->plagueTimer = 0;
          scbw::removeOverlays(unit, ImageId::PlagueOverlay_Small, ImageId::PlagueOverlay_Large);
        }
      }


      //짐 레이너 마린에 특수 공격 추가
      if (unit->id == UnitId::jim_raynor_marine && unit->spellCooldown == 0) {
        //공격 대상이 건물인지 확인
        if (unit->orderTarget.unit && unit->orderTarget.unit->status & UnitStatus::GroundedBuilding) {
          //공격 중인지 확인
          const u8 currentAnim = unit->sprite->mainGraphic->animation;
          if (currentAnim == IscriptAnimation::Unused2
              || currentAnim == IscriptAnimation::GndAttkInit
              || currentAnim == IscriptAnimation::GndAttkRpt) {
            scbw::fireUnitWeapon(unit, WeaponId::FragmentationGrenade); //무기 ID == 4
            unit->playIscriptAnim(IscriptAnimation::Unused2);
            unit->spellCooldown = 30;
            unit->groundWeaponCooldown = 30;
          }
        }
      }

      //짐 레이너 벌처에 특수 공격 추가
      if (unit->id == UnitId::jim_raynor_vulture && unit->spellCooldown == 0) {
        //공격 대상이 건물인지 확인
        if (unit->orderTarget.unit && unit->orderTarget.unit->status & UnitStatus::GroundedBuilding) {
          //공격 중인지 확인
          const u8 currentAnim = unit->sprite->mainGraphic->animation;
          if (currentAnim == IscriptAnimation::CastSpell
              || currentAnim == IscriptAnimation::GndAttkInit
              || currentAnim == IscriptAnimation::GndAttkRpt) {
            scbw::fireUnitWeapon(unit, 128);                            //무기 ID == 128
            unit->playIscriptAnim(IscriptAnimation::CastSpell);
            unit->spellCooldown = 30;
            unit->groundWeaponCooldown = 30;
          }
        }
      }


      //까치날개 포탑이 벙커에 안 붙어 있으면 자폭
      if (unit->id == UnitId::physics_lab && unit->connectedUnit == NULL)
        unit->mainOrderId = OrderId::Die;

      //벙커에 포탑 달기 및 회수 기능 추가
      if (unit->id == UnitId::bunker) {
        //까치날개 포탑 달기
        if (unit->mainOrderId == OrderId::CarrierStop) {
          unit->currentButtonSet = 119;
          unit->building.addonBuildType = UnitId::physics_lab;          //까치날개 포탑 ID
          unit->mainOrderId = OrderId::PlaceAddon;
          unit->orderTarget.pt.x = unit->position.x;
          unit->orderTarget.pt.y = unit->position.y;
          if (unit->building.addon)
            unit->building.addon->connectedUnit = unit;                 //생성된 포탑 유닛과 연결
        }

        //회수
        if (unit->mainOrderId == OrderId::ReaverStop) {
          unit->currentButtonSet = UnitId::None;
          //미네랄 75% 되받기
          resources->playerMin[unit->playerId] += (int) (Unit::MineralCost[unit->id] * 0.75);
          //안에 있는 모든 유닛 내림
          unit->orderTo(OrderId::Unload, unit);
          //컨트롤 불가능
          unit->status |= UnitStatus::Disabled & UnitStatus::CanNotReceiveOrders;
          //같이 달려있는 애드온(포탑)도 자폭
          if (unit->building.addon) {
            CUnit* addon = unit->building.addon;
            addon->mainOrderId = OrderId::Die;
            resources->playerMin[addon->playerId] += (int) (Unit::MineralCost[addon->id] * 0.75);
          }
          //회수 시간
          unit->removeTimer = 40;
        }
      }


      //커맨드 센터를 궤도 사령부나 행성 요새로 업글
      if (unit->id == UnitId::command_center
          && unit->secondaryOrderId == OrderId::Train) {
        //생산 예약된 유닛의 ID 확인
        const u16 currentTrainingUnitId = unit->buildQueue[0];
        if (currentTrainingUnitId == UnitId::lair
            || currentTrainingUnitId == UnitId::hive) {
          unit->mainOrderId = OrderId::Morph2;
          unit->displayedUnitId = currentTrainingUnitId;
          unit->secondaryOrderId = OrderId::Nothing2;
        }
      }


      //누클리어 사일로에 핵이 장전되어 있을 시 이미지 변화
      if (unit->id == UnitId::nuclear_silo
          && (unit->status & UnitStatus::Completed)) {
        CImage* const unitGraphic = unit->sprite->mainGraphic;
        if (unit->building.silo.hasNuke && unitGraphic->frameSet == 0)
          scbw::playFrame(unitGraphic, 2);
        else if (!unit->building.silo.hasNuke && unitGraphic->frameSet == 2)
          scbw::playFrame(unitGraphic, 0);
      }


      //테란 유닛의 킬수에 따른 계급 증가
      if (!(unit->status & UnitStatus::IsBuilding)
          && Unit::GroupFlags[unit->id].isTerran) {
        if (unit->killCount >= 20) unit->rankIncrease = 15;
        else if (unit->killCount >= 15) unit->rankIncrease = 10;
        else if (unit->killCount >= 10) unit->rankIncrease = 6;
        else if (unit->killCount >= 5) unit->rankIncrease = 4;
      }


      //배틀크루저에 자체 디펜시브 스킬 추가
      if (unit->id == UnitId::battlecruiser
          && unit->mainOrderId == OrderId::CarrierStop) {
        unit->orderTo(OrderId::DefensiveMatrix, unit);
      }


      //토르를 실은 수송 유닛의 이미지 변화
      if (unit->id == 56) { //쿠쿨자 가디언을 토르로 사용
        CUnit* const transport = unit->connectedUnit;
        if (transport != NULL
            && (transport->id == UnitId::dropship|| transport->id == 8 || transport->id == 57)
            && !(transport->mainOrderId == OrderId::Unload
                 || transport->mainOrderId == OrderId::MoveUnload)
            && transport->sprite->mainGraphic->frameSet < 17)
        {
          scbw::playFrame(transport->sprite->mainGraphic, 17);
        }
      }


      //넥서스에 시간 증폭 스킬 추가
      if (unit->id == UnitId::nexus
          && unit->mainOrderId == OrderId::PlaceScanner) {
        CUnit* const target = unit->orderTarget.unit;
        if (target != NULL
            && (target->status & (UnitStatus::GroundedBuilding | UnitStatus::Completed))
            && target->playerId == unit->playerId
            && !(target->id == UnitId::photon_cannon        //시증을 걸 수 없는 건물
                 || target->id == UnitId::pylon
                 || target->id == UnitId::shield_battery
                 || target->id == UnitId::assimilator)
            && target->id >= 154 && target->id <= 172) {
          scbw::createOverlay(unit->sprite, 269);
          target->unusedTimer = 50;                         //시간 증폭 지속 시간 타이머
          unit->mainOrderId = OrderId::Nothing2;
          unit->orderTarget.unit = NULL;
          //에너지 사용
          if (!scbw::isCheatEnabled(CheatFlags::TheGathering))
            unit->energy -= Tech::EnergyCost[TechId::ScannerSweep] * 256;
        }
        else {
          unit->orderTo(OrderId::CastFeedback, unit);       //자기 자신에게 피드백을 쓰게 해서 오류 메시지 출력
        }
      }
    } //end of for loop

    // Loop through the bullet table.
    // Warning: There is no guarantee that the current bullet is actually a
    // bullet rather than an unused space in memory
    //for (int i = 0; i < BULLET_ARRAY_LENGTH; ++i) {
    //  BULLET* bullet = &bulletTable[i];
    //  //Write your code here
    //}

    // Alternative looping method
    // Guarantees that [bullet] points to an actual bullet.
    //for (BULLET* bullet = *firstBullet; bullet; bullet = bullet->next) {
    //  //Write your code here
    //}
  }
  return true;
}

bool gameOn() {
  firstRun = true;
  setMaxSightRange<50>();   //최대 시야를 50으로 늘림
  return true;
}

bool gameEnd() {
  return true;
}
