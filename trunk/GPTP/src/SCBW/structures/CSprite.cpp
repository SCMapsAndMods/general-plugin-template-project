#include "CSprite.h"
#include "CImage.h"
#include <cassert>

//Functionally identical to playSpriteIscript() (offset 0x00499D00)
void CSprite::playIscriptAnim(IscriptAnimation::Enum animation) {
  assert(this);
  for (CImage *img = this->imageHead; img; img = img->next)
    img->playIscriptAnim(animation);
}
