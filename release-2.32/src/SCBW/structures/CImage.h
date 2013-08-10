//Based on BWAPI's BW/CBullet.h

#pragma once
#include "Position.h"
#pragma pack(1)

struct CSprite;

struct CImage {
  CImage      *previous;
  CImage      *next;
  u16         id;
  u8          paletteType;      //++Drawing function (values are the same as DataEdit)
                                // officially "ubRLE"
                                // RLE_SHADOW = 10
                                // RLE_HPFLOATDRAW = 11
                                // RLE_OUTLINE = 13
                                // RLE_PLAYER_SIDE = 14
  u8          direction;
  u16         flags;
              /*  0x0001  - Redraw
                  0x0002  - Don't update x?
                  0x0004  - Don't update y?
                  0x0008  -
                  0x0010  -
                  0x0020  -
                  0x0040  - Hidden/Invisible (don't draw)
                  0x0080  -
                  0x0100  -
                  0x0200  -
                  0x0400  -
                  0x0800  -
                  0x1000  -
                  0x2000  -
                  0x4000  -
                  0x8000  -
              */
  u8          horizontalOffset;
  u8          verticalOffset;
  u16         iscriptHeaderOffset;
  u16         iscriptOffset;
  UNK         unknown2[2];
  u8          animation;
  u8          wait;
  u16         frameSet;
  u16         frameIndex;
  Position    mapPosition;
  Position    screenPosition;
  UNK         unknown3[8];
  u32         grpOffset;
  void*       coloringData;        //?
  void*       renderFunction;
  void*       updateFunction;
  CSprite     *parentSprite;
};

C_ASSERT(sizeof(CImage) == 64);

#pragma pack()
