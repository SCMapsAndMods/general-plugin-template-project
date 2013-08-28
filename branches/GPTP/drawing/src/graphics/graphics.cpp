#include "graphics.h"
#include "Shape.h"
#include "../SCBW/scbwdata.h"

namespace graphics {

const int MAX_SHAPES  = 10000;
const int MAX_STRINGS = 2000;

//Static arrays for fast drawing
Shape shapes[MAX_SHAPES];
std::string strings[MAX_STRINGS];
int shapeCount = 0;
int stringCount = 0;

void resetAllGraphics() {
  shapeCount = 0;
  stringCount = 0;
}

void drawTextOnScreen(int x, int y, const std::string& str) {
  strings[stringCount] = str;
  shapes[shapeCount++].setText(x, y, stringCount++);
}
//void drawTextOnMap(int x, int y, const std::string& str);

void drawDotOnScreen(int x, int y, ColorId color) {
  shapes[shapeCount++].setDot(x, y, color);
}
//void drawDotOnMap(int x, int y, ColorId color);

void drawLineOnScreen(int x1, int y1, int x2, int y2, ColorId color) {
  shapes[shapeCount++].setLine(x1, y1, x2, y2, color);
}
//void drawLineOnMap(int x1, int y1, int x2, int y2, ColorId color);

void drawBoxOnScreen(int left, int top, int right, int bottom, ColorId color) {
  shapes[shapeCount++].setBox(left, top, right, bottom, color);
}
//void drawBoxOnMap(int left, int top, int right, int bottom, ColorId color);

void drawCircleOnScreen(int x, int y, int radius, ColorId color) {
  shapes[shapeCount++].setCircle(x, y, radius, color);
}
//void drawCircleOnMap(int x, int y, int radius, ColorId color);

void drawFilledBoxOnScreen(int left, int top, int right, int bottom, ColorId color) {
  shapes[shapeCount++].setFilledBox(left, top, right, bottom, color);
}
//void drawFilledBoxOnMap(int left, int top, int right, int bottom, ColorId color);

void drawFilledCircleOnScreen(int x, int y, int radius, ColorId color) {
  shapes[shapeCount++].setFilledCircle(x, y, radius, color);
}
//void drawFilledCircleOnMap(int x, int y, int radius, ColorId color);

} //graphics