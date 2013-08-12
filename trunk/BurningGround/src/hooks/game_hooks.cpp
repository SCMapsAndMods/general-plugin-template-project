/// This is where the magic happens; program your plug-in's core behavior here.

#include "game_hooks.h"
#include "../SCBW/api.h"
#include "../SCBW/scbwdata.h"
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
