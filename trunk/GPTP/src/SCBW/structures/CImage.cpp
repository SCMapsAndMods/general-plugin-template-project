#include "CImage.h"
#include "CSprite.h"
#include "../api.h"
#include "../scbwdata.h"
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
  if (parent->imageHead == this)
    parent->imageHead = this->link.next;
  if (parent->imageTail == this)
    parent->imageTail = this->link.prev;

  if (this->link.prev)
    this->link.prev->link.next = this->link.next;
  if (this->link.next)
    this->link.next->link.prev = this->link.prev;

  this->link.prev = NULL;
  this->link.next = NULL;
  this->grpOffset = NULL;

  unusedImages.insertAfterHead(this);
}
