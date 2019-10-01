#include "background.hpp"


gameBackground::gameBackground(GLfloat x, GLfloat y, int type, spriteManagerClass* sprMan)
{
  switch(type)
  {
    case 0:
      sprite = sprMan->spawnByFile( "gfx/editor/spec-starbg.png" );
    break;
    case 1:
      sprite = sprMan->spawnByFile( "gfx/editor/spec-bluebg.png" );
    break;
    case 2:
      sprite = sprMan->spawnByFile( "gfx/editor/spec-redbg.png" );
    break;
    case 3:
      sprite = sprMan->spawnByFile( "gfx/editor/spec-toxbg.png" );
    break;
  }
  sprite->position(x,y);
};
