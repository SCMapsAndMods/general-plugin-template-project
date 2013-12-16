#include "bunker_hooks.h"
#include <SCBW/scbwdata.h>
#include "../SCBW/enumerations.h"
#include <SCBW/api.h>

//Helper function declarations. Do NOT modify!
namespace {

CThingy* createThingy(u16 spriteId, s16 x, s16 y, s8 playerId = 0);
void setImageDirection(CImage *image, s8 direction);
void setThingyVisibilityFlags(CThingy *thingy);

} //unnamed namespace

namespace hooks {

/// Checks whether the unit can attack from inside a bunker.
bool unitCanAttackInsideBunkerHook(const CUnit *unit) {
  //인페스티드 테란과 러커는 제외
	//버그 방지를 위함
  if (unit->id == UnitId::infested_terran || unit->id == UnitId::lurker)
    return false;

  const u8 groundWeapon = Unit::GroundWeapon[unit->id];
  const u8 airWeapon = Unit::AirWeapon[unit->id];

  //무기가 없으면 공격 못하게 함
  if (groundWeapon == WeaponId::None && airWeapon == WeaponId::None)
    return false;

  //지상 무기, 공중 무기 중 사거리가 어느 정도 길어야만 공격 가능
  if (unit->getMaxWeaponRange(groundWeapon) < 32
      || unit->getMaxWeaponRange(airWeapon) < 32)
    return false;

  return true;
}

void createBunkerAttackThingyHook(const CUnit *unit) {
  CImage *bunkerImage = unit->connectedUnit->sprite->mainGraphic;

  u8 frameDirection = (unit->currentDirection1 + 16) / 32 % 8;
  const LO_Header *loFile = lo_files->attackOverlays[bunkerImage->id];
  Point8 offset = loFile->getOffset(bunkerImage->frameIndex, frameDirection);
  
  if (bunkerImage->flags & 0x2) //Is inverted
    offset.x = -offset.x;

  u8 frameAngle;
  u16 spriteId;
  bool isBlueFlame = false;

  if (unit->id == UnitId::firebat || unit->id == UnitId::gui_montag) {
    frameAngle = ((unit->currentDirection1 + 8) / 16 % 16) * 16;
    spriteId = 378; //Firebat flamethrower graphics

    //파이어뱃 불꽃 업그레이드 체크
	if (scbw::getUpgradeLevel(unit->playerId, UPGRADE_FIREBAT_BLUE_FLAME)&&unit->id!=UnitId::gui_montag)//파이어뱃일때만 파란불꽃
      isBlueFlame = true;
  }
  //침 뱉는 유닛들
  else if (unit->id == UnitId::hydralisk
           || unit->id == UnitId::hunter_killer
           || unit->id == UnitId::drone)
  {
    frameAngle = ((unit->currentDirection1 + 8) / 16 % 16) * 16;
    spriteId = 332; //히드라, 드론이 침뱉는 이미지
  }
  //총 쏘는 유닛들
  else if (unit->id == UnitId::marine
           || unit->id == UnitId::jim_raynor_marine
           || unit->id == UnitId::civilian
           || unit->id == UnitId::ghost
           || unit->id == UnitId::sarah_kerrigan
           || unit->id == UnitId::alexei_stukov
           || unit->id == UnitId::samir_duran
           || unit->id == UnitId::Hero_InfestedDuran)
  {
    frameAngle = frameDirection * 32;
    spriteId = 377; //Bunker attack overlay
  }
  else
    return;

  CThingy *bunkerAttackEffect = createThingy(spriteId,
                                             offset.x + unit->getX(),
                                             offset.y + unit->getY());
  if (!bunkerAttackEffect) return;

  bunkerAttackEffect->sprite->elevationLevel = unit->sprite->elevationLevel + 1;
  for (CImage *image = bunkerAttackEffect->sprite->imageHead;
       image; image = image->link.next) {
    setImageDirection(image, frameAngle);
  }
  setThingyVisibilityFlags(bunkerAttackEffect);

  //파이어뱃 파란불꽃 설정
  if (isBlueFlame)
    bunkerAttackEffect->sprite->mainGraphic->setRemapping(ColorRemapping::BExpl);
}

} //hooks


//-------- Helper function definitions. Do NOT modify! --------//

namespace {

const u32 Func_CreateThingy = 0x00488210;
CThingy* createThingy(u16 spriteId, s16 x, s16 y, s8 playerId) {
  static CThingy *thingy;
  s32 x_ = x, playerId_ = playerId;
  u32 spriteId_ = spriteId;

  __asm {
    PUSHAD
    PUSH playerId_
    MOV DI, y
    PUSH x_
    PUSH spriteId_
    CALL Func_CreateThingy
    MOV thingy, EAX
    POPAD
  }

  return thingy;
}

const u32 Func_SetImageDirection = 0x004D5F80;
void setImageDirection(CImage *image, s8 direction) {
  u32 direction_ = direction;

  __asm {
    PUSHAD
    PUSH direction_
    MOV ESI, image
    CALL Func_SetImageDirection
    POPAD
  }
}

const u32 Func_SetThingyVisibilityFlags = 0x004878F0;
void setThingyVisibilityFlags(CThingy *thingy) {
  __asm {
    PUSHAD
    MOV ESI, thingy
    CALL Func_SetThingyVisibilityFlags
    POPAD
  }
}

};