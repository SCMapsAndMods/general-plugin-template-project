#pragma once
#include <string>
#include "../types.h"

namespace graphics {

//Should move to graphics.cpp
const int MAX_SHAPES  = 10000;
const int MAX_STRINGS = 2000;

enum CoordType { SCREEN, MAP };

void drawText(int x, int y, const std::string* str, CoordType target = SCREEN);
void drawDot(int x, int y, u8 color, CoordType target = SCREEN);
void drawLine(int x1, int y1, int x2, int y2, u8 color, CoordType target = SCREEN);
void drawBox(int x1, int y1, int x2, int y2, u8 color, CoordType target = SCREEN);
void drawRect(int x1, int y1, int width, int height, u8 color, CoordType target = SCREEN);
void drawCircle(int x1, int y1, int radius, u8 color, CoordType target = SCREEN);
void drawFilledBox(int x1, int y1, int x2, int y2, u8 color, CoordType target = SCREEN);
void drawFilledCircle(int x1, int y1, int radius, u8 color, CoordType target = SCREEN);

} //graphics
