#ifndef ASTROID_HPP_INCLUDED
#define ASTROID_HPP_INCLUDED

#include "../ents.hpp"
class astroid : public gameObject
{
  private:
    GLfloat rotation, rv;
  public:
    astroid(spriteClass* spr);
    void sim();
};

#endif // ASTROID_HPP_INCLUDED
