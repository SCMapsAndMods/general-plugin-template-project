#pragma once
#include "../types.h"
#include "ColorId.h"
#include "FontSize.h"
#include <string>

namespace graphics {

//Speed-efficient non-virtual class for storing shapes that will be drawn.
//All shape setter methods merely copy data and contain no actual logic.
//All coordinates are relative to screen.
class Shape {
  public:
    void setText(int x, int y, int stringIndex, FontSize fontSize);
    void setDot(int x, int y, ColorId color);
    void setLine(int x1, int y1, int x2, int y2, ColorId color);
    void setBox(int left, int top, int right, int bottom, ColorId color);
    void setCircle(int x, int y, int radius, ColorId color);
    void setFilledBox(int left, int top, int right, int bottom, ColorId color);
    void setFilledCircle(int x, int y, int radius, ColorId color);
    void draw() const;

  private:
    enum ShapeType {
      NONE,
      TEXT,
      DOT,
      LINE,
      BOX,
      CIRCLE,
      FILLED_BOX,
      FILLED_CIRCLE
    } type;
    POINT p1;
    POINT p2;
    int radius;
    ColorId color;
    FontSize fontSize;
    int stringIndex;
};

} //graphics
