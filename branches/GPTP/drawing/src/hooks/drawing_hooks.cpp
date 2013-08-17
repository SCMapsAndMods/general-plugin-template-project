#include "drawing_hooks.h"
#include "../SCBW/api.h"

void customDrawing() {
  gameScreenBuffer->blitString("Hello, world!", 20, 20, 2);
  gameScreenBuffer->blitString("Sorry, no Korean support as of yet (죄송합니다)", 50, 50, 3);
  for (int i = 0; i <= 3; ++i)
    gameScreenBuffer->blitString("Welcome to GameDrawing.....", 200 + 10 * i, 10 + i * 50, i);
}