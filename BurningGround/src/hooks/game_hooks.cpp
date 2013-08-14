/// This is where the magic happens; program your plug-in's core behavior here.

#include "game_hooks.h"
#include "../SCBW/api.h"
#include "../SCBW/scbwdata.h"
#include "../SCBW/enumerations.h"
#include "../SCBW/ExtendSightLimit.h"
#include "../SCBW/utilities.h"
#include <cstdio>

bool firstRun = true;

//유닛이 걸어다닐 수 있으며 충돌 크기가 있는지 확인
bool unitIsGroundWalkable(const CUnit* const unit) {
  using UnitStatus::GroundedBuilding;
  using UnitStatus::InAir;
  using UnitStatus::IgnoreTileCollision;
  using UnitStatus::NoCollide;
  using UnitStatus::IsGathering;
  using UnitStatus::Disabled;
  using UnitStatus::CanNotReceiveOrders;

  return unit->status & UnitStatus::Completed
         && !(unit->status & (GroundedBuilding | InAir | NoCollide | IsGathering | Disabled | CanNotReceiveOrders | IgnoreTileCollision))
         && !(Unit::BaseProperty[unit->id] & UnitProperty::Subunit);
}

//비켜서기 기능 (search된 유닛을 위해 unit이 비켜줌)
void stepAsideForOthers(CUnit* const unit) {
  const int RADIUS = 37;  //탐색 거리
  CUnit* closestSearch = NULL;
  int closestDistance = RADIUS;

  if (!unitIsGroundWalkable(unit)) return;
  if (unit->mainOrderId != OrderId::PlayerGuard
      && unit->mainOrderId != OrderId::Medic)
      return;

  //가장 가까운 유닛 찾기
  const int unitX = unit->getX(), unitY = unit->getY();
  for (CUnit* search = *firstVisibleUnit; search; search = search->next) {
    if (unit->playerId == search->playerId && unit != search
        && unitIsGroundWalkable(search)
        && search->sprite->mainGraphic->animation == IscriptAnimation::Walking) {
      int distance = scbw::getDistanceFast(unitX, unitY, search->getX(), search->getY());
      if (distance <= closestDistance) {
        closestDistance = distance;
        closestSearch = search;
      }
    }
  }
  if (closestSearch == NULL) return;

  //비켜주기
  u16 moveX = unit->orderTarget.pt.x, moveY = unit->orderTarget.pt.y;
  if (unitX > closestSearch->getX()) moveX += 10;
  else moveX -= 10;
  if (unitY > closestSearch->getY()) moveX += 10;
  else moveY -= 10;

  if (unit->id == UnitId::sarah_kerrigan)
    unit->unusedTimer = 4;                                  //???

  if (unit->id == UnitId::medic)
    unit->orderTo(OrderId::HealMove, moveX, moveY);
  else
    unit->orderTo(OrderId::Move, moveX, moveY);
}

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

      //지상 유닛 비켜주기
      stepAsideForOthers(unit);

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

      //시간 증폭이 걸린 건물의 업글 및 유닛 생산 가속
      if (unit->status & (UnitStatus::GroundedBuilding | UnitStatus::Completed)
          && Unit::GroupFlags[unit->id].isProtoss
          && unit->unusedTimer > 0) {
        //시증 효과는 항상 생성
        if (!scbw::hasOverlay(unit, 264)
            || unit->sprite->underlay->frameSet >= 17)
          scbw::createOverlay(unit->sprite, 264);
        //실제 효과는 건물에 파일런 동력이 공급될 때만 (2프레임당 1번)
        if (!(unit->status & UnitStatus::DoodadStatesThing)
            && unit->cycleCounter % 2) {
          //엄그레이드 및 테크 가속
          if (unit->building.upgradeResearchTime > 0)
            unit->building.upgradeResearchTime--;
          //유닛 생산 가속
          CUnit* const trainingUnit = unit->currentBuildUnit;
          if (trainingUnit && trainingUnit->remainingBuildTime > 1) {
            trainingUnit->remainingBuildTime--;
            trainingUnit->hitPoints = 102400;               //400HP로 강제 지정
          }
        }
      }


      //핵 미사일에 그래픽 효과 추가
      //pastelmind: 이거 iscript.bin으로 돌리는 게 좋을 것 같은데?
      if (unit->id == UnitId::nuclear_missile
          && unit->status & UnitStatus::Completed
          && unit->sprite->mainGraphic->animation == IscriptAnimation::Walking
          && !scbw::hasOverlay(unit, 27)) {
        scbw::createOverlay(unit->sprite, 27);
      }


      //바이킹 변신 관련, 공격 애니메이션이 따로 놀 때 수정
      //pastelmind: 이거 DatEdit에서 건드리면 고칠 수 있을 텐데...?
      if (unit->id == UnitId::edmund_duke
          && unit->status & UnitStatus::Completed
          && unit->subunit
          && unit->sprite->mainGraphic->animation == IscriptAnimation::Walking) {
        CImage* const subunitImage = unit->subunit->sprite->mainGraphic;
        if (subunitImage->animation == IscriptAnimation::GndAttkInit
            || subunitImage->animation == IscriptAnimation::GndAttkRpt)
          unit->mainOrderId = OrderId::Stop;
      }


      //배럭, 팩토리, 스타포트가 애드온으로 이동 시 애드온 타입 자동 변경
      //@TODO: 확실한 로직 물어보기
      if (unit->mainOrderId == OrderId::Follow) {
        CUnit* const targetAddon = unit->orderTarget.unit;
        if (targetAddon && targetAddon->playerId == 11
            && (targetAddon->id == UnitId::machine_shop
                || targetAddon->id == UnitId::covert_ops
                || targetAddon->id == UnitId::control_tower)
            ) {
          u16 changeId;
          switch (unit->id) {
            case UnitId::barracks:
              changeId = UnitId::covert_ops; break;
            case UnitId::factory:
              changeId = UnitId::machine_shop; break;
            case UnitId::starport:
              changeId = UnitId::control_tower; break;
            default:
              changeId = UnitId::None;
          }
          if (changeId != UnitId::None) {
            targetAddon->id = changeId;
            targetAddon->currentButtonSet = changeId;
          }
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
