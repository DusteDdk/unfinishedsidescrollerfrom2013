#include "ammo.hpp"

#include <iostream>
using namespace std;
ammoClass::ammoClass()
{
  sprite=NULL;
  dead=0;
}

void ammoClass::draw()
{
  if(sprite)
  {
    sprite->position(pos.x,pos.y - sprite->halfSize.y);
    sprite->draw();
  }
}

void ammoClass::sim()
{
  pos.x += vel.x;
  pos.y += vel.y;
}
