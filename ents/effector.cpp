#include "effector.hpp"

//Ctor for the editor showing
effectorClass::effectorClass(GLfloat x, GLfloat y, spriteManagerClass* sprMan)
{
  p.x=x;
  p.y=y;
  sprite= sprMan->spawnByFile( "gfx/editor/fx.tex" );
  sprite->position(p);
}
