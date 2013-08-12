/// This is where the magic happens; program your plug-in's core behavior here.

#include "game_hooks.h"
#include "../SCBW/api.h"
#include "../SCBW/scbwdata.h"
#include "../SCBW/enumerations.h"
#include "../SCBW/ExtendSightLimit.h"
#include <cstdio>

bool firstRun = true;

/// This hook is called every frame; most of your plugin's logic goes here.
bool nextFrame() {
	if (!scbw::isGamePaused()) { //If the game is not paused

		if (firstRun) {
			scbw::printText("Hello, world!");
			firstRun = false;
		}

		// Loop through the unit table.
    // Warning: There is no guarantee that the current unit is actually a unit
    // rather than an unused space in memory.
		//for (int i = 0; i < UNIT_ARRAY_LENGTH; ++i) {
		//	CUnit* unit = &unitTable[i];
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
          resources->playerMin[unit->playerId] += Unit::MineralCost[unit->id] * 0.75;
          //안에 있는 모든 유닛 내림
          unit->orderTo(OrderId::Unload, unit);
          //컨트롤 불가능
          unit->status |= UnitStatus::Disabled & UnitStatus::CanNotReceiveOrders;
          //같이 달려있는 애드온(포탑)도 자폭
          if (unit->building.addon) {
            CUnit* addon = unit->building.addon;
            addon->mainOrderId = OrderId::Die;
            resources->playerMin[addon->playerId] += Unit::MineralCost[addon->id] * 0.75;
          }
          //자폭 시간
          unit->removeTimer = 40;
        }
      }

    }

		// Loop through the bullet table.
    // Warning: There is no guarantee that the current bullet is actually a
    // bullet rather than an unused space in memory
		//for (int i = 0; i < BULLET_ARRAY_LENGTH; ++i) {
		//	BULLET* bullet = &bulletTable[i];
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
