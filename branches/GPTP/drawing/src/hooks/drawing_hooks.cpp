#include "drawing_hooks.h"
#include "../SCBW/api.h"
#include <cstdio>
#include <vector>

std::vector<std::string> string_vector;

void flushString() {
  string_vector.clear();
}

void addString(const std::string& str) {
  string_vector.push_back(str);
}

int customDrawing() {
  gameScreenBuffer->blitString("Hello, world!", 20, 20, 2);
  char buffer[100];
  sprintf(buffer, "string_vector has %d elements", string_vector.size());
  gameScreenBuffer->blitString(buffer, 200, 20, 1);
  
  int startY = 50;
  int drawCount = 0;
  while (drawCount < string_vector.size()) {
    gameScreenBuffer->blitString(string_vector[drawCount].c_str(), 40, startY, 2);
    startY += 20;
    ++drawCount;
  }

  return drawCount;
}