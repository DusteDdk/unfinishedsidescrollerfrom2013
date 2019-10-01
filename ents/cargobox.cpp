#include "cargobox.hpp"

cargoBoxInSpace::cargoBoxInSpace(spriteClass* spr)
{
  item=NULL;
  sprite = spr;
  rotation=rand()%360;
  rv=0;
  //astroids might rotate
  if(rand()%100 < 70)
  {
    rv = rand()%360;
    rv -=180;
    rv /=100;
  }
}

void cargoBoxInSpace::draw()
{
  rotation += rv;
  sprite->rotate(rotation);
  sprite->draw();
}
