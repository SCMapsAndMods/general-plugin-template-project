#include "CImage.h"
#include "CSprite.h"
#include <SCBW/api.h>
#include <SCBW/scbwdata.h>
#include <cassert>

void CImage::playIscriptAnim(IscriptAnimation::Enum animation) {
  assert(this);
  u32 animation_ = (u8)animation;

  const u32 Func_PlayIscriptAnim = 0x004D8470;  //AKA playImageIscript();
  __asm {
    PUSHAD
    PUSH animation_
    MOV ECX, this
    CALL Func_PlayIscriptAnim
    POPAD
  }
}

void CImage::free() {
  assert(this);
  if (!(screenLayers->game.hasBeenRefreshed))
    scbw::refreshScreen(this->screenPosition.x,
                        this->screenPosition.y,
                        this->screenPosition.x + this->grpSize.right,
                        this->screenPosition.y + this->grpSize.bottom
                        );

  CSprite* const parent = this->parentSprite;
  const CListExtern<CImage, &CImage::link>
    imageList(parent->imageHead, parent->imageTail);

  imageList.unlink(this);
  this->grpOffset = NULL;

  unusedImages.insertAfterHead(this);
}

//Loosely based on code at @ 0x004D5A50
void CImage::setRemapping(ColorRemapping::Enum remapping) {
  assert(this);
  this->coloringData = colorShift[remapping].data;
}
