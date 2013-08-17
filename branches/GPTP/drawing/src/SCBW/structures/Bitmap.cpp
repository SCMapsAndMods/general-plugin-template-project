//Based on BWAPI's BW/Bitmap.cpp
#include "Bitmap.h"
#include "Font.h"
#include "../scbwdata.h"
#include <cassert>

u16 Bitmap::getWidth() const {
  assert(this);
  return this->width;
}

u16 Bitmap::getHeight() const {
  assert(this);
  return this->height;
}

u8 gbColorTable[] = {
   0,  0,  0,  1,  2,  3,  4,  5,  8,  0,  0,  0,  0,  0,  9, 10,
  11, 12,  0,  0,  0, 13, 14, 15, 16, 17,  0, 18, 19, 20, 21, 23
};

u8 gbFontColors[24][8] = {
  { 0xC0, 0x9B, 0x9A, 0x95, 0x43, 0x00, 0x00, 0x28 },
  { 0x56, 0xA7, 0x6D, 0x65, 0x5C, 0x00, 0x00, 0x8A },
  { 0x41, 0xFF, 0x53, 0x97, 0x47, 0x00, 0x00, 0x8A },
  { 0x40, 0x96, 0x49, 0x90, 0x42, 0x00, 0x00, 0x8A },
  { 0xA8, 0xAE, 0x17, 0x5E, 0xAA, 0x00, 0x00, 0x8A },
  { 0xB5, 0x75, 0xBA, 0xB9, 0xB7, 0x00, 0x00, 0x8A },
  { 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08 },
  { 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08 },
  { 0x8A, 0x6F, 0x17, 0x5E, 0xAA, 0x8A, 0x8A, 0x8A },
  { 0x28, 0xA5, 0xA2, 0x2D, 0xA0, 0x8A, 0x8A, 0x8A },
  { 0x8A, 0x9F, 0x9E, 0x9D, 0xB7, 0x8A, 0x8A, 0x8A },
  { 0x8A, 0xA4, 0xA3, 0xA1, 0x0E, 0x8A, 0x8A, 0x8A },
  { 0x8A, 0x9C, 0x1C, 0x1A, 0x13, 0x8A, 0x8A, 0x8A },
  { 0x8A, 0x13, 0x12, 0x11, 0x57, 0x8A, 0x8A, 0x8A },
  { 0x8A, 0x54, 0x51, 0x4E, 0x4A, 0x8A, 0x8A, 0x8A },
  { 0x8A, 0x87, 0xA6, 0x81, 0x93, 0x8A, 0x8A, 0x8A },
  { 0xB5, 0xB9, 0xB8, 0xB7, 0xB6, 0x8A, 0x8A, 0x8A },
  { 0x8A, 0x88, 0x84, 0x81, 0x60, 0x8A, 0x8A, 0x8A },
  { 0x8A, 0x86, 0x72, 0x70, 0x69, 0x8A, 0x8A, 0x8A },
  { 0x8A, 0x33, 0x7C, 0x7A, 0xA0, 0x8A, 0x8A, 0x8A },
  { 0x8A, 0x4D, 0x26, 0x23, 0x22, 0x8A, 0x8A, 0x8A },
  { 0x8A, 0x9A, 0x97, 0x95, 0x91, 0x8A, 0x8A, 0x8A },
  { 0x8A, 0x88, 0x84, 0x81, 0x60, 0x8A, 0x8A, 0x8A },
  { 0x8A, 0x80, 0x34, 0x31, 0x2E, 0x8A, 0x8A, 0x8A }
};

bool Bitmap::blitString(const char *pszStr, int x, int y, u8 size) {
  // verify valid size index
  if (size > 3 || !pszStr)
    return false;

  // localize pointer
  Font *fnt = fontBase[size];
  if (!fnt)
    return false;

  // verify if drawing should be done
  assert(x + fnt->getTextWidth(pszStr) >= 0);
  assert(y + fnt->getTextHeight(pszStr) >= 0);
  assert(x < this->getWidth());
  assert(y < this->getHeight());
  if (x + fnt->getTextWidth(pszStr)  < 0 ||
      y + fnt->getTextHeight(pszStr) < 0 ||
      x >= this->getWidth() || y >= this->getHeight())
    return false;

  // Reference an unsigned character array
  const u8 *pbChars = (u8*) pszStr;

  u8 lastColor = 0, color = 0;
  int Xoffset = x, Yoffset = y;

  // Iterate all characters in the message
  for (int c = 0; pbChars[c]; ++c) {
    // Perform control character and whitespace functions
    if (pbChars[c] <= ' ') {
      switch (pbChars[c]) {
      case 1:       // restore last colour
        color = lastColor;
        continue;
      case '\t':    // 9    tab
        Xoffset += fnt->getCharWidth( pbChars[c] );
        continue;
      case '\n':    // 10   newline
        Xoffset = x;
        Yoffset += fnt->maxHeight();
        continue;
      case 11:      // invisible
      case 20:
        color = (BYTE)~0;
        continue;
      case '\f':    // 12   formfeed
        break;
      case '\r':    // 13   carriage return
      case 26:
        continue;
      case 18:      // right align
        Xoffset += this->getWidth() - fnt->getTextWidth(pszStr) - x;
        continue;
      case 19:      // center align
        Xoffset += (this->getWidth() - fnt->getTextWidth(pszStr))/2 - x;
        continue;
      case ' ':     // space
        Xoffset += fnt->maxWidth() / 2;
        continue;
      default:      // colour code
        lastColor = color;
        color     = gbColorTable[ pbChars[c] ];
        continue;
      }
    }

    // Skip if the character is not supported by the font
    FontChar *fntChr = fnt->getChar( pbChars[c] );
    if (!fntChr)
      continue;

    // If the colour is not "invisible"
    if ( color != ~0 )
    {
      // begin drawing character process
      for ( int i = 0, pos = 0; pos < fntChr->getHeight() * fntChr->getWidth(); ++i, ++pos )
      {
        // font position
        pos += fntChr->pixelOffset(i);

        // x offset
        int newX = Xoffset + (fntChr->getX() + pos % fntChr->getWidth());
        if ( newX >= this->getWidth() ) break;
        if ( newX < 0 ) continue;

        // y offset
        int newY = Yoffset + (fntChr->getY() + pos / fntChr->getWidth());
        if ( newY >= this->getHeight() ) break;
        if ( newY < 0 ) continue;

        // blit offset
        int offset = newY * this->getWidth() + newX;
        if ( offset >= this->getWidth() * this->getHeight() ) break;
        if ( offset < 0 ) continue;

        // Plot pixel
        this->data[offset] = gbFontColors[color][fntChr->colorMask(i)];
      }
    } // invis colour

    // Increment the X offset for the width of the character
    Xoffset += fntChr->getWidth();
  }
  return true;
}
