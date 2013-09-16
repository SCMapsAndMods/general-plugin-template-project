/// Graphics module for GPTP, adapted from BWAPI.

#pragma once
#include <string>
#include "../types.h"

namespace graphics {

//Based on BWAPI's ColorId.h and ColorId.cpp
//No strongly typed enum because we're at pre-C++11
typedef u8 ColorId;
const ColorId RED     = 111;  //Player 1 color
const ColorId BLUE    = 165;  //Player 2 color
const ColorId TEAL    = 159;  //Player 3 color
const ColorId PURPLE  = 164;  //Player 4 color
const ColorId ORANGE  = 179;  //Player 5 color
const ColorId BROWN   = 19;   //Player 6 color
const ColorId WHITE   = 255;  //Player 7 color
const ColorId YELLOW  = 135;  //Player 8 color
const ColorId GREEN   = 117;  //Player 7 color for Ice tilesets
const ColorId CYAN    = 128;  //Neutral player color
const ColorId BLACK   = 0;
const ColorId GREY    = 74;

//Font sizes accepted by graphics::drawTextOnScreen() and drawTextOnMap().
enum FontSize {
  FONT_SMALL    = 0,
  FONT_MEDIUM   = 1,
  FONT_LARGE    = 2,
  FONT_LARGEST  = 3,
};

/// Resets all previous graphics and clears error messages. This should be
/// called every frame in game_hooks.cpp. 
void resetAllGraphics();

void drawTextOnScreen(int x, int y, const std::string& str,
                      FontSize fontSize = FONT_MEDIUM);
void drawTextOnMap(int x, int y, const std::string& str,
                   FontSize fontSize = FONT_MEDIUM);

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
