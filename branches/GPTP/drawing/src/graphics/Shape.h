#pragma once
#include "../types.h"

namespace graphics {

/// Speed-efficient POD class for storing shapes that will be drawn.
class Shape {
  public:
    void setText();
    void setDot();
    void setLine();
    void setBox();
    void setCircle();
    void setFilledBox();
    void setFilledCircle();

  private:
    enum ShapeType {
      None,
      Text,
      Dot,
      Line,
      Box,
      Circle,
      FilledBox,
      FilledCircle
    } type;
    POINT p1;
    POINT p2;
    int radius;
    u8 color;
    int stringIndex;
};

} //graphics