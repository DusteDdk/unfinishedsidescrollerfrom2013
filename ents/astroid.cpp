#include "astroid.hpp"

astroid::astroid(spriteClass* spr)
{

  sprite = spr;

  rotation=0;
  rv = 0;
  rotation=rand()%360;
  rv=0;
  //astroids might rotate
  if(rand()%100 < 70)
  {
    rv = rand()%360;
    rv -=180;
    rv /=100;
  }
  //Set a random frame as the first
  sprite->frame = rand()%sprite->numFrames;

}

void astroid::sim()
{
  if(velocity.x || velocity.y)
  {
    p.x +=velocity.x;
    p.y +=velocity.y;
    position(p);
  }
  rotation += rv;
  sprite->rotate(rotation);
}
