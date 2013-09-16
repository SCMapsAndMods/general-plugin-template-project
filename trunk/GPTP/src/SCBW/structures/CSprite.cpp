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

  const int y = CLAMP(this->position.y / 32, 0, mapTileSize->height - 1);
  const CListExtern<CSprite, &CSprite::link>
    spritesOnTileRow(spritesOnTileRow->heads[y],
                     spritesOnTileRow->tails[y]);

  spritesOnTileRow.unlink(this);
  unusedSprites.insertAfterHead(this);
}

void CSprite::setPosition(u16 x, u16 y) {
  assert(this);
  if (this->position.x == x && this->position.y == y) return;

  const int oldTileY = CLAMP(this->position.y / 32, 0, mapTileSize->height - 1);
  const int newTileY = CLAMP(y / 32, 0, mapTileSize->height - 1);

  this->position.x = x;
  this->position.y = y;

  if (oldTileY != newTileY) {
    const CListExtern<CSprite, &CSprite::link>
      spritesOnOldTileRow(spritesOnTileRow->heads[oldTileY],
                          spritesOnTileRow->tails[oldTileY]),
      spritesOnNewTileRow(spritesOnTileRow->heads[newTileY],
                          spritesOnTileRow->tails[newTileY]);

    spritesOnOldTileRow.unlink(this);
    spritesOnNewTileRow.insertAfterHead(this);
  }

  for (CImage *i = this->imageHead; i; i = i->link.next)
    i->flags |= 1;
}
