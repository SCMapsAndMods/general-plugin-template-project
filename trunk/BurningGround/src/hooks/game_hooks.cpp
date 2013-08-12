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

      //���� II�� ��Ű���� ���ٿ�, �����̽ý�, ��Ƽ��, �ν��׾�, �̷���, �÷��̱׿� ���� �������� ����
      //hooks/update_status_effects.cpp�� ����
      if (unit->id == UnitId::norad_ii || unit->id == UnitId::valkyrie) {
        if (unit->lockdownTimer) unit->removeLockdown();
        if (unit->stasisTimer) unit->removeStasisField();
        if (unit->isBlind) unit->isBlind = 0;

        //�ν��׾� ����
        if (unit->ensnareTimer) {
          unit->ensnareTimer = 0;
          scbw::removeOverlays(unit, ImageId::EnsnareOverlay_Small, ImageId::EnsnareOverlay_Large);
          unit->updateSpeed();
        }

        //�̷�����Ʈ ����
        if (unit->irradiateTimer) {
          unit->irradiateTimer = 0;
          scbw::removeOverlays(unit, ImageId::Irradiate_Small, ImageId::Irradiate_Large);
          unit->irradiatedBy = NULL;
          unit->irradiatePlayerId = 8;
        }

        //�÷��̱� ����
        if (unit->plagueTimer) {
          unit->plagueTimer = 0;
          scbw::removeOverlays(unit, ImageId::PlagueOverlay_Small, ImageId::PlagueOverlay_Large);
        }
      }


      //�� ���̳� ������ Ư�� ���� �߰�
      if (unit->id == UnitId::jim_raynor_marine && unit->spellCooldown == 0) {
        //���� ����� �ǹ����� Ȯ��
        if (unit->orderTarget.unit && unit->orderTarget.unit->status & UnitStatus::GroundedBuilding) {
          //���� ������ Ȯ��
          const u8 currentAnim = unit->sprite->mainGraphic->animation;
          if (currentAnim == IscriptAnimation::Unused2
              || currentAnim == IscriptAnimation::GndAttkInit
              || currentAnim == IscriptAnimation::GndAttkRpt) {
            scbw::fireUnitWeapon(unit, WeaponId::FragmentationGrenade); //���� ID == 4
            unit->playIscriptAnim(IscriptAnimation::Unused2);
            unit->spellCooldown = 30;
            unit->groundWeaponCooldown = 30;
          }
        }
      }

      //�� ���̳� ��ó�� Ư�� ���� �߰�
      if (unit->id == UnitId::jim_raynor_vulture && unit->spellCooldown == 0) {
        //���� ����� �ǹ����� Ȯ��
        if (unit->orderTarget.unit && unit->orderTarget.unit->status & UnitStatus::GroundedBuilding) {
          //���� ������ Ȯ��
          const u8 currentAnim = unit->sprite->mainGraphic->animation;
          if (currentAnim == IscriptAnimation::CastSpell
              || currentAnim == IscriptAnimation::GndAttkInit
              || currentAnim == IscriptAnimation::GndAttkRpt) {
            scbw::fireUnitWeapon(unit, 128);                            //���� ID == 128
            unit->playIscriptAnim(IscriptAnimation::CastSpell);
            unit->spellCooldown = 30;
            unit->groundWeaponCooldown = 30;
          }
        }
      }


      //��ġ���� ��ž�� ��Ŀ�� �� �پ� ������ ����
      if (unit->id == UnitId::physics_lab && unit->connectedUnit == NULL)
        unit->mainOrderId = OrderId::Die;

      //��Ŀ�� ��ž �ޱ� �� ȸ�� ��� �߰�
      if (unit->id == UnitId::bunker) {
        //��ġ���� ��ž �ޱ�
        if (unit->mainOrderId == OrderId::CarrierStop) {
          unit->currentButtonSet = 119;
          unit->building.addonBuildType = UnitId::physics_lab;          //��ġ���� ��ž ID
          unit->mainOrderId = OrderId::PlaceAddon;
          unit->orderTarget.pt.x = unit->position.x;
          unit->orderTarget.pt.y = unit->position.y;
          if (unit->building.addon)
            unit->building.addon->connectedUnit = unit;                 //������ ��ž ���ְ� ����
        }

        //ȸ��
        if (unit->mainOrderId == OrderId::ReaverStop) {
          unit->currentButtonSet = UnitId::None;
          //�̳׶� 75% �ǹޱ�
          resources->playerMin[unit->playerId] += Unit::MineralCost[unit->id] * 0.75;
          //�ȿ� �ִ� ��� ���� ����
          unit->orderTo(OrderId::Unload, unit);
          //��Ʈ�� �Ұ���
          unit->status |= UnitStatus::Disabled & UnitStatus::CanNotReceiveOrders;
          //���� �޷��ִ� �ֵ��(��ž)�� ����
          if (unit->building.addon) {
            CUnit* addon = unit->building.addon;
            addon->mainOrderId = OrderId::Die;
            resources->playerMin[addon->playerId] += Unit::MineralCost[addon->id] * 0.75;
          }
          //���� �ð�
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
  setMaxSightRange<50>();   //�ִ� �þ߸� 50���� �ø�
	return true;
}

bool gameEnd() {
	return true;
}
