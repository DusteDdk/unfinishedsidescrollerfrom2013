#ifndef ITEMINFOHEADER
#define ITEMINFOHEADER

#include <iostream>
#include "sprite.hpp"

class itemInfo {
  public:
    int price; //How many credits is required to purchase this
    string infoTxt; //The description of this item
    spriteClass* infoPic; //A pointer to the sprite containing the animation of this item, shown on the shop screen
    spriteClass* sprite; //A pointer to the "ingame" "idle" sprite
};
#endif
