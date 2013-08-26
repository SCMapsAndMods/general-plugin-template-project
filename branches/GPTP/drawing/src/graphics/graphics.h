#pragma once
#include <string>
#include "../types.h"

namespace graphics {

//Should move to graphics.cpp
const int MAX_SHAPES  = 10000;
const int MAX_STRINGS = 2000;

//TODO: Replace [toMap] with proper enums (CoordType?)
void drawText(int x, int y, const std::string* str, bool toMap = true);
void drawDot(int x, int y, u8 color, bool toMap = true);
void drawLine(int x1, int y1, int x2, int y2, u8 color, bool toMap = true);
void drawBox(int x1, int y1, int x2, int y2, u8 color, bool toMap = true);
void drawRect(int x1, int y1, int width, int height, u8 color, bool toMap = true);
void drawCircle(int x1, int y1, int radius, u8 color, bool toMap = true);
void drawFilledBox(int x1, int y1, int x2, int y2, u8 color, bool toMap = true);
void drawFilledCircle(int x1, int y1, int radius, u8 color, bool toMap = true);

} //graphics
