#include "CImage.h"
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
