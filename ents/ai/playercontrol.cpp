#include "playercontrol.hpp"


playerControlClass::playerControlClass(inputClass* i)
{
  inp=i;
}

void playerControlClass::run()
{

  //dampen
  if(c->vel[1] < 0.0)
  {
    c->vel[1] -= c->vel[1]/14.0;
    if(c->vel[1] > 0.0)
      c->vel[1]=0;
  }

  if(c->vel[1] > 0.0)
  {
    c->vel[1] -= c->vel[1]/14.0;
    if(c->vel[1] < 0.0)
      c->vel[1]=0;
  }

  //x is dampened by camcontrol

  if(inp->keyStates[SDLK_UP])
  {
    c->vel[1] -= 1.0;
  } else if(inp->keyStates[SDLK_DOWN])
  {
    c->vel[1] += 1.0;
  }
  if(inp->keyStates[SDLK_LEFT])
  {
    c->vel[0] -= 1;
  } else if(inp->keyStates[SDLK_RIGHT])
  {
    c->vel[0] += 1;
  }


  //
  if(c->vel[0] < -10.0)
  {
    c->vel[0] = -10.0;
  }
  if(c->vel[0] > 14)
  {
    c->vel[0]=14;
  }


}
