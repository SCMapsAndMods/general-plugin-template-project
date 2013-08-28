#include "Shape.h"

namespace graphics {

void Shape::setText(int x, int y, int stringIndex) {
  this->type = TEXT;
  this->p1.x = x;
  this->p1.y = y;
  this->stringIndex = stringIndex;
}

void Shape::setDot(int x, int y, ColorId color) {
  this->type = DOT;
  this->p1.x = x;
  this->p1.y = y;
  this->color = color;
}

void Shape::setLine(int x1, int y1, int x2, int y2, ColorId color) {
  this->type = LINE;
  this->p1.x = x1;
  this->p1.y = y1;
  this->p2.x = x2;
  this->p2.y = y2;
  this->color = color;
}

void Shape::setBox(int left, int top, int right, int bottom, ColorId color) {
  this->type = BOX;
  this->p1.x = left;
  this->p1.y = top;
  this->p2.x = right;
  this->p2.y = bottom;
  this->color = color;
}

void Shape::setCircle(int x, int y, int radius, ColorId color) {
  this->type = CIRCLE;
  this->p1.x = x;
  this->p1.y = y;
  this->radius = radius;
  this->color = color;
};

void Shape::setFilledBox(int left, int top, int right, int bottom, ColorId color) {
  this->type = FILLED_BOX;
  this->p1.x = left;
  this->p1.y = top;
  this->p2.x = right;
  this->p2.y = bottom;
  this->color = color;
};

void Shape::setFilledCircle(int x, int y, int radius, ColorId color) {
  this->type = FILLED_CIRCLE;
  this->p1.x = x;
  this->p1.y = y;
  this->radius = radius;
  this->color = color;
};

} //graphics