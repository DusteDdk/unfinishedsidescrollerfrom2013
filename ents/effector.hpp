#ifndef EFFECTOR_HPP_INCLUDED
#define EFFECTOR_HPP_INCLUDED

#include "../ents.hpp"

class effectorClass : public gameObject
{
  public:
    effectorClass(GLfloat x, GLfloat y, spriteManagerClass* sprMan);
    effectorClass() {}
};

#endif // EFFECTOR_HPP_INCLUDED
