#pragma once
#include "../types.h"

namespace graphics {

typedef u8 ColorId;

//Based on BWAPI's ColorId.h and ColorId.cpp
//No strongly typed enum because we're at pre-C++11

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

} //graphics
