#pragma once
#include <string>
#include "../types.h"
#include "ColorId.h"

namespace graphics {

/// Resets all previous graphics and clears error messages. This should be
/// called every frame in game_hooks.cpp. 
void resetAllGraphics();

void drawTextOnScreen(int x, int y, const std::string& str);
void drawTextOnMap(int x, int y, const std::string& str);

void drawDotOnScreen(int x, int y, ColorId color);
void drawDotOnMap(int x, int y, ColorId color);

void drawLineOnScreen(int x1, int y1, int x2, int y2, ColorId color);
void drawLineOnMap(int x1, int y1, int x2, int y2, ColorId color);

void drawBoxOnScreen(int left, int top, int right, int bottom, ColorId color);
void drawBoxOnMap(int left, int top, int right, int bottom, ColorId color);

void drawCircleOnScreen(int x, int y, int radius, ColorId color);
void drawCircleOnMap(int x, int y, int radius, ColorId color);

void drawFilledBoxOnScreen(int left, int top, int right, int bottom, ColorId color);
void drawFilledBoxOnMap(int left, int top, int right, int bottom, ColorId color);

void drawFilledCircleOnScreen(int x, int y, int radius, ColorId color);
void drawFilledCircleOnMap(int x, int y, int radius, ColorId color);

} //graphics
