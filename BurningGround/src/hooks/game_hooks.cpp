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
  const int RADIUS = 50;  //탐색 거리
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
  u16 moveX = unitX, moveY = unitY;
  if (unitX > closestSearch->getX()) moveX += 10;
  else moveX -= 10;
  if (unitY > closestSearch->getY()) moveY += 10;
  else moveY -= 10;

  if (unit->id == UnitId::sarah_kerrigan)
    unit->unusedTimer = 4;                                  //???

  if (unit->id == UnitId::medic)
    unit->orderTo(OrderId::HealMove, moveX, moveY);
  else
    unit->orderTo(OrderId::Move, moveX, moveY);
}


//자동 수리를 할 수 있는 유닛인지 확인
bool unitCanRepair(const CUnit *unit) {
  return unit->id == UnitId::scv
         || unit->id == UnitId::science_vessel
         || unit->id == UnitId::magellan
         || unit->id == UnitId::drone;
}

//수리를 받을 수 있는 유닛인지 확인
bool unitCanBeRepaired(const CUnit *unit) {
  using Unit::BaseProperty;
  using Unit::GroupFlags;
  using Unit::MaxHitPoints;

  if (unit->status & UnitStatus::Completed
      && unit->hitPoints < (MaxHitPoints[unit->id] * 256)) {
    if (BaseProperty[unit->id] & UnitProperty::Mechanical && GroupFlags[unit->id].isTerran)
      return true;
    else if (unit->status & (UnitStatus::GroundedBuilding | UnitStatus::InAir))
      return true;
  }
  return false;
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


      //자동 수리 기능
      if (unitCanRepair(unit)) {
        //자동 수리 켜기
        if (unit->mainOrderId == OrderId::HoldPosition2 && !unit->unused_0x106) {
          unit->unused_0x106 = 1;
          scbw::createOverlay(unit->sprite, 976);
        }
        //자동 수리 끄기
        if (unit->mainOrderId == OrderId::Stop && unit->unused_0x106) {
          unit->unused_0x106 = 0;
          scbw::removeOverlays(unit, 976);
          if (unit->connectedUnit) unit->connectedUnit = NULL;
        }
        //여기부터는 무슨 로직인지 잘 모르겠음.
        //따라다니는 유닛을 우선적으로 치료한다...?
        if (unit->mainOrderId == OrderId::Move
          || unit->mainOrderId == OrderId::AttackMove
          || unit->mainOrderId == OrderId:: Harvest1
          || unit->mainOrderId == OrderId:: AttackUnit)
            if(unit->connectedUnit)
              unit->connectedUnit = NULL;
        if (unit->mainOrderId == OrderId::PlayerGuard && unit->unused_0x106)
          unit->mainOrderId = OrderId::HoldPosition2;
        if (unit->mainOrderId == OrderId::Follow && unit->unused_0x106 && unit->orderTarget.unit) {
          if (unit->connectedUnit == NULL)
            unit->connectedUnit = unit->orderTarget.unit;
          if (unit->connectedUnit && unit->connectedUnit != unit->orderTarget.unit)
            unit->connectedUnit = unit->orderTarget.unit;
        }
        if (unit->mainOrderId == OrderId::HoldPosition2 && unit->connectedUnit) {
          unit->mainOrderId = OrderId::Follow;
          unit->orderTarget.unit = unit->connectedUnit;
        }
        //실제로 치료할 대상을 탐색
        CUnit *bestRepairTarget = NULL;
        int bestDistance = 150; //최대 거리
        const u16 unitX = unit->getX(), unitY = unit->getY();
        if (unit->unused_0x106) {
          if (unit->mainOrderId == OrderId::HoldPosition2
            || unit->mainOrderId == OrderId::AttackMove
            || unit->mainOrderId == OrderId::Follow) {
            for (CUnit* repairTarget = *firstVisibleUnit; repairTarget; repairTarget = repairTarget->next) {
              if (unit != repairTarget && unit->playerId == repairTarget->playerId
                  && unitCanBeRepaired(repairTarget)) {
                const int distance = scbw::getDistanceFast(unitX, unitY, repairTarget->getX(), repairTarget->getY());
                if (distance < bestDistance) {
                  bestRepairTarget = repairTarget;
                  bestDistance = distance;
                }
              }
            }
          }
        }
        if (bestRepairTarget)
          unit->orderTo(OrderId::Repair1, bestRepairTarget);
      }


      //서플라이 디포 내리기/올리기 (위에 다른 유닛이 없을 때만 올릴 수 있음)
      if (unit->id == UnitId::supply_depot && unit->mainOrderId == OrderId::Stop) {
        unit->mainOrderId = OrderId::Guard;
        //올리기
        if (unit->status & UnitStatus::NoCollide) {
          //위에 다른 지상 유닛이 겹쳐 있는지 확인
          bool isCollide = false;
          for (CUnit *collideUnit = *firstVisibleUnit; collideUnit; collideUnit = collideUnit->next) {
            if (!(collideUnit->status & (UnitStatus::Burrowed | UnitStatus::InAir))
                && !(Unit::BaseProperty[collideUnit->id] & UnitProperty::Subunit)
                && unit->getLeft() <= collideUnit->getRight()
                && unit->getRight() >= collideUnit->getLeft()
                && unit->getTop() <= collideUnit->getBottom()
                && unit->getBottom() >= collideUnit->getTop()
                && unit != collideUnit) {
              isCollide = true;
              break;
            }
          }
          if (!isCollide) {
            scbw::playIscriptAnim(unit->sprite->mainGraphic, IscriptAnimation::UnBurrow);
            unit->status &= ~(UnitStatus::NoCollide);
            unit->sprite->elevationLevel = Unit::Elevation[unit->id];
            unit->currentButtonSet = UnitId::supply_depot;  //서플의 버튼셋
          }
        }
        //내리기
        else {
          scbw::playIscriptAnim(unit->sprite->mainGraphic, IscriptAnimation::Burrow);
          unit->status |= UnitStatus::NoCollide;
          unit->sprite->elevationLevel = 0;
          unit->currentButtonSet = 103; //내려진 서플의 버튼셋
        }
      }


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
          unit->building.addonBuildType = UnitId::physics_lab;          //까치날개 포탑 ID
          unit->orderTo(OrderId::PlaceAddon, unit->position.x, unit->position.y);
        }
        //포탑이 생성되었을 경우 버튼셋 변경
        if (unit->mainOrderId == OrderId::PlaceAddon && unit->building.addon
            && unit->currentButtonSet != 119)
          unit->currentButtonSet = 119;

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
      if (Unit::BaseProperty[unit->id] & UnitProperty::Building
          && unit->mainOrderId == OrderId::Follow) {
        CUnit* const targetAddon = unit->orderTarget.unit;
        u16 changeId = UnitId::None;
        if (targetAddon && targetAddon->playerId == 11) {
          //도미니언 건물
          if (targetAddon->id == UnitId::Special_IonCannon
              || targetAddon->id == UnitId::machine_shop
              || targetAddon->id == UnitId::control_tower) {
            switch (unit->id) {
              case UnitId::barracks:
                changeId = UnitId::Special_IonCannon; break;
              case UnitId::factory:
                changeId = UnitId::machine_shop; break;
              case UnitId::starport:
                changeId = UnitId::control_tower; break;
              default: break;
            }
          }
          //켈모리안 건물
          else if (targetAddon->id == UnitId::covert_ops
            || targetAddon->id == UnitId::Special_IndependentStarport
            || targetAddon->id == UnitId::UnusedIndependentJumpGate) {
            switch (unit->id) {
              case UnitId::spawning_pool:
                changeId = UnitId::covert_ops; break;
              case UnitId::ultralisk_cavern:
                changeId = UnitId::Special_IndependentStarport; break;
              case UnitId::UnusedIndependentCommandCenter:
                changeId = UnitId::UnusedIndependentJumpGate; break;
              default: break;
            }
          }
          if (changeId != UnitId::None) {
            targetAddon->id = changeId;
            targetAddon->currentButtonSet = changeId;
          }
        }
      }


      //짐 레이너 마린의 클로킹?
      if (unit->id == UnitId::jim_raynor_marine) {
        CImage* const mainImg = unit->sprite->mainGraphic;
        if (mainImg->animation == IscriptAnimation::Walking) {
          if (unit->secondaryOrderId != OrderId::Cloak && unit->unusedTimer == 0) {
            scbw::createOverlay(unit->sprite, 978);
            unit->status |= UnitStatus::CloakingForFree;
            unit->secondaryOrderId = OrderId::Cloak;
          }
        }
        else {
          if (unit->secondaryOrderId == OrderId::Cloak) {
            scbw::createOverlay(unit->sprite, 978);
            unit->status &= ~(UnitStatus::CloakingForFree);
            unit->secondaryOrderId = OrderId::Decloak;
          }
        }
        if (mainImg->animation == IscriptAnimation::GndAttkInit
            || mainImg->animation == IscriptAnimation::GndAttkRpt
            || mainImg->animation == IscriptAnimation::Unused2)
           unit->unusedTimer = 22;
      }


      //스팀팩 사용 시작 및 종료시 그래픽 효과 추가
      if (unit->status & UnitStatus::Completed) {
        if (unit->stimTimer == 40)
          scbw::createOverlay(unit->sprite, 975);
        else if (unit->stimTimer == 1)
          scbw::createOverlay(unit->sprite, 63);
      }

    } //end of for loop

    // Loop through hidden units
    for (CUnit *hiddenUnit = *firstHiddenUnit; hiddenUnit; hiddenUnit = hiddenUnit->next) {
      //토르를 실은 수송 유닛의 이미지 변화
      //쿠쿨자 가디언을 토르로 사용
      if (hiddenUnit->id == 56 && hiddenUnit->status & UnitStatus::InTransport) {
        CUnit* const transport = hiddenUnit->connectedUnit;
        if (transport != NULL
            && (transport->id == UnitId::dropship || transport->id == 8 || transport->id == 57)
            && !(transport->mainOrderId == OrderId::Unload
                 || transport->mainOrderId == OrderId::MoveUnload)
            && transport->sprite->mainGraphic->frameSet < 17)
        {
          scbw::playFrame(transport->sprite->mainGraphic, 17);
        }
      }
    } //end of hidden unit loop

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
