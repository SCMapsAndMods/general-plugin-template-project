//Based on BWAPI's BW/CBullet.h

#pragma once
#include "common.h"
#include "CList.h"
#include <SCBW/enumerations.h>
#pragma pack(1)

struct CSprite;
struct GrpHead;

struct CImage {
//Utility method definitions added by pastelmind


  /// Makes the image play the specified Iscript animation entry.
  void playIscriptAnim(IscriptAnimation::Enum animation);

  /// AKA ImageDestructor() @ 0x004D4CE0
  /// Removes the image from the linked list of valid images. This is different
  /// from a class destructor, since CImage objects are stored in a static array
  /// and never truly destroyed.
  void free();

  /// Set the color remapping of this image to @ remapping.
  void setRemapping(ColorRemapping::Enum remapping);

  /// Resets most of the data member values. This is meant to be used in other
  /// functions, and should not be used by modders.
  void initializeData(CSprite *parent, u16 imageId, s8 x, s8 y);


////////////////////////////////////////////////////////////////
//Actual data structure
  CLink<CImage> link;
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
  Point16     mapPosition;
  Point16     screenPosition;
  Box16       grpSize;
  GrpHead     *grpOffset;
  void*       coloringData;        //?
  void*       renderFunction;
  void*       updateFunction;
  CSprite     *parentSprite;
};

static_assert(sizeof(CImage) == 64, "The size of the CImage structure is invalid");

#pragma pack()
