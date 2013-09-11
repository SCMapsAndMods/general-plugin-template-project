#include "CSprite.h"
#include "CImage.h"
#include "../scbwdata.h"
#include <algorithm>
#include <cassert>

//Functionally identical to playSpriteIscript() (offset 0x00499D00)
void CSprite::playIscriptAnim(IscriptAnimation::Enum animation) {
  assert(this);
  for (CImage *img = this->imageHead; img; img = img->link.next)
    img->playIscriptAnim(animation);
}

void CSprite::free() {
  assert(this);
  CImage *image = this->imageHead;
  while (image) {
    CImage *nextImage = image->link.next;
    image->free();
    image = nextImage;
  }

  int y = this->position.y / 32;
  y = std::min(std::max(y, 0), mapTileSize->height - 1);
  if (spritesOnTile->arr1[y] == this)
    spritesOnTile->arr1[y] = this->link.prev;
  if (spritesOnTile->arr2[y] == this)
    spritesOnTile->arr2[y] = this->link.next;

  if (this->link.prev)
    this->link.prev->link.next = this->link.next;
  if (this->link.next)
    this->link.next->link.prev = this->link.prev;

  this->link.prev = NULL;
  this->link.next = NULL;

  unusedSprites.insertAfterHead(this);
}
