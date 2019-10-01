#ifndef BACKGROUND_HPP_INCLUDED
#define BACKGROUND_HPP_INCLUDED

#include "../ents.hpp"

class gameBackground : public gameObject
{
  public:
    gameBackground( GLfloat x, GLfloat y, int type,spriteManagerClass* sprMan );
};

#endif // BACKGROUND_HPP_INCLUDED
