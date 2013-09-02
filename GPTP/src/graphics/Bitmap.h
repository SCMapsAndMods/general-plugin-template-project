//Based on BWAPI's BW/Bitmap.h
#pragma once
#include "ColorId.h"
#pragma pack(1)

namespace graphics {

class Bitmap {
  public:
    bool blitString(const char *pszStr, int x, int y, u8 size);
    u16 getWidth() const;
    u16 getHeight() const;

	void drawDot(int x, int y, ColorId color);
	void drawLine(int x1, int y1, int x2, int y2, ColorId color);
	void drawBox(int left, int top, int right, int bottom, ColorId color);
	void drawFilledBox(int left, int top, int right, int bottom, ColorId color);
  void drawCircle(int x, int y, int radius, ColorId color);
  void drawFilledCircle(int x, int y, int radius, ColorId color);

  private:
    u16 width, height;
    u8* data;

	void drawHorizontalLine(int x1, int x2, int y, ColorId color);
	void drawVerticalLine(int x, int y1, int y2, ColorId color);
	void drawDotUnsafe(int x, int y, ColorId color);
	void drawHorizontalLineUnsafe(int x1, int x2, int y, ColorId color);
	void drawVerticalLineUnsafe(int x, int y1, int y2, ColorId color);
};

} //graphics

#pragma pack()
