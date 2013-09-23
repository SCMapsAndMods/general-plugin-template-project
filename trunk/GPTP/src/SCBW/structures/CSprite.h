//Based on BWAPI's BW/CSprite.h

#pragma once
#include "common.h"
#include "CList.h"
#include "../enumerations.h"
#pragma pack(1)

struct CImage;

struct CSprite {
//Utility method definitions added by pastelmind


  /// Makes the sprite play the specified Iscript animation entry.
  void playIscriptAnim(IscriptAnimation::Enum animation);

  /// AKA SpriteDestructor() @ 0x00497B40
  /// Removes the sprite from the linked list of valid images. This is different
  /// from a class destructor, since CSprite objects are stored in a static
  /// array and never truly destroyed.
  void free();

  /// AKA fixSpritePosition @ 0x00497A10
  /// Moves the sprite to the new position.
  void setPosition(u16 x, u16 y);

  /// Creates an image overlay over the main image of this sprite.
  CImage* createOverlay(u32 imageId, s8 x = 0, s8 y = 0, u32 direction = 0);

  /// Creates an image overlay on top of all other images of this sprite.
  CImage* createTopOverlay(u32 imageId, s8 x = 0, s8 y = 0, u32 direction = 0);


////////////////////////////////////////////////////////////////
//Actual data structure

  /*0x00*/ CLink<CSprite> link;
  /*0x08*/ u16       spriteID;
  /*0x0A*/ u8        playerID;          // officially "creator"
  /*0x0B*/ u8        selectionIndex;    // 0 <= selectionIndex <= 11. Index in the selection area at bottom of screen.
  /*0x0C*/ u8        visibilityFlags;
  /*0x0D*/ u8        elevationLevel;
  /*0x0E*/ u8        flags;
                     /*   0x01  Draw selection circle.
                          0x02
                          0x04
                          0x08  Selected.
                          0x10
                          0x20  Hidden
                          0x40  Burrowed
                          0x80  Iscript unbreakable code section.
                     */
  /*0x0F*/ u8        selectionTimer;
  /*0x10*/ u32       index;
  /*0x14*/ Point16   position;
  /*0x18*/ CImage    *mainGraphic;    // officially "pImagePrimary"
  /*0x1C*/ CImage    *imageHead;      // officially "pImageHead"
  /*0x20*/ CImage    *imageTail;      // officially "pImageTail"

  ////////////////////////////////////////////////////////////////////
  // Official Broodwar methods (from beta), ignore these
  /*
  bool  Dead() const;
  bool  HasDamageFrames() const;
  */
};

C_ASSERT(sizeof(CSprite) == 36);

#pragma pack()
