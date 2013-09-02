#include <cassert>
#include "graphics.h"
#include "graphics_errors.h"
#include "graphics_misc.h"
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
  clearErrors();
}


void drawTextOnScreen(int x, int y, const std::string& str) {
  if (stringCount >= MAX_STRINGS) {
    setError(ERR_TOO_MANY_STRINGS);
    return;
  }
  if (shapeCount >= MAX_SHAPES) {
    setError(ERR_TOO_MANY_SHAPES);
    return;
  }
  strings[stringCount] = str;
  shapes[shapeCount++].setText(x, y, stringCount++);
}

void drawTextOnMap(int x, int y, const std::string& str) {
  drawTextOnScreen(x - (*screenX), y - (*screenY), str);
}


void drawDotOnScreen(int x, int y, ColorId color) {
  if (shapeCount >= MAX_SHAPES) {
    setError(ERR_TOO_MANY_SHAPES);
    return;
  }
  shapes[shapeCount++].setDot(x, y, color);
}

void drawDotOnMap(int x, int y, ColorId color) {
  drawDotOnScreen(x - (*screenX), y - (*screenY), color);
}


void drawLineOnScreen(int x1, int y1, int x2, int y2, ColorId color) {
  if (shapeCount >= MAX_SHAPES) {
    setError(ERR_TOO_MANY_SHAPES);
    return;
  }
  shapes[shapeCount++].setLine(x1, y1, x2, y2, color);
}

void drawLineOnMap(int x1, int y1, int x2, int y2, ColorId color) {
  drawLineOnScreen(x1 - (*screenX), y1 - (*screenY),
                   x2 - (*screenX), y2 - (*screenY), color);
}


void drawBoxOnScreen(int left, int top, int right, int bottom, ColorId color) {
  if (shapeCount >= MAX_SHAPES) {
    setError(ERR_TOO_MANY_SHAPES);
    return;
  }
  shapes[shapeCount++].setBox(left, top, right, bottom, color);
}

void drawBoxOnMap(int left, int top, int right, int bottom, ColorId color) {
  drawBoxOnScreen(left  - (*screenX), top    - (*screenY),
                  right - (*screenX), bottom - (*screenY), color);
}


void drawCircleOnScreen(int x, int y, int radius, ColorId color) {
  if (shapeCount >= MAX_SHAPES) {
    setError(ERR_TOO_MANY_SHAPES);
    return;
  }
  shapes[shapeCount++].setCircle(x, y, radius, color);
}

void drawCircleOnMap(int x, int y, int radius, ColorId color) {
  drawCircleOnScreen(x - (*screenX), y - (*screenY), radius, color);
}


void drawFilledBoxOnScreen(int left, int top, int right, int bottom, ColorId color) {
  if (shapeCount >= MAX_SHAPES) {
    setError(ERR_TOO_MANY_SHAPES);
    return;
  }
  shapes[shapeCount++].setFilledBox(left, top, right, bottom, color);
}

void drawFilledBoxOnMap(int left, int top, int right, int bottom, ColorId color) {
  drawFilledBoxOnScreen(left  - (*screenX), top    - (*screenY),
                        right - (*screenX), bottom - (*screenY), color);
}


void drawFilledCircleOnScreen(int x, int y, int radius, ColorId color) {
  if (shapeCount >= MAX_SHAPES) {
    setError(ERR_TOO_MANY_SHAPES);
    return;
  }
  shapes[shapeCount++].setFilledCircle(x, y, radius, color);
}

void drawFilledCircleOnMap(int x, int y, int radius, ColorId color) {
  drawFilledCircleOnScreen(x - (*screenX), y - (*screenY), radius, color);
}


const std::string& getStringFromIndex(int stringIndex) {
  assert(0 <= stringIndex && stringIndex < MAX_STRINGS);
  return strings[stringIndex];
}

int drawAllShapes() {
  int drawnShapeCount = 0;
  for (int i = 0; i < shapeCount; ++i)
    shapes[i].draw();
  drawErrorMessages();
  return shapeCount;  //TODO: Fix this
}

} //graphics