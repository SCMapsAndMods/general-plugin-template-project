//Based on BWAPI's BW/Font.cpp
#include "Font.h"
#include "../SCBW/scbwdata.h"
#include <cassert>

namespace graphics {

FontChar* Font::getChar(char c) const {
  assert(this);
  u8 ch = (u8)c;
  if (this->low <= ch && ch <= this->high
      && this->chrs[ch - this->low] != (FontChar*)this)
    return this->chrs[ch - this->low];
  return NULL;
}

int Font::getCharWidth(char c) const {
  assert(this);
  switch (c) {
    case '\t':
      return this->maxWidth() * 2;
    case ' ':
      return this->maxWidth() / 2;
    default:
      return this->getChar(c)->getWidth();
  }
}

// ----------------- GET WIDTH ---------------------
int Font::getTextWidth(const char *pszStr) const {
  // verify valid size index
  assert(this);
  if (!pszStr) return 0;

  // Retrieve size
  int size = 0;
  for (; *pszStr; ++pszStr)
    size += this->getCharWidth(*pszStr);
  return size;
}

// ----------------- GET WIDTH ---------------------
int Font::getTextHeight(const char *pszStr) const {
  // verify valid size index
  assert(this);
  if (!pszStr) return 0;

  int size = this->yMax;
  for (; *pszStr; ++pszStr) {
    if (*pszStr == '\n')
      size += this->yMax;
  }
  return size;
}

int Font::maxWidth() const {
  assert(this);
  return this->xMax;
}

int Font::maxHeight() const {
  assert(this);
  return this->yMax;
}

int Font::getTextWidth(const char *pszStr, int size) {
  assert(0 <= size && size <= 3);
  return fontBase[size]->getTextWidth(pszStr);
}

int Font::getTextHeight(const char *pszStr, int size) {
  assert(0 <= size && size <= 3);
  return fontBase[size]->getTextHeight(pszStr);
}

// ---------------- FontChar ----------------

int FontChar::getWidth() const {
  if (this != NULL)
    return this->width;
  else
    return 0;
}

int FontChar::getHeight() const {
  assert(this);
  return this->height;
}

int FontChar::getX() const {
  assert(this);
  return this->x;
}

int FontChar::getY() const {
  assert(this);
  return this->y;
}

int FontChar::colorMask(int index) const {
  assert(this);
  return this->data[index] & 7;
}

int FontChar::pixelOffset(int index) const {
  assert(this);
  return this->data[index] >> 3;
}

} //graphics
