#ifndef CARGOBOX_HPP_INCLUDED
#define CARGOBOX_HPP_INCLUDED

#include "../ents.hpp"

class cargoBoxInSpace : public gameObject
{
  private:
    GLfloat rotation, rv;
    int item; //0=empty
  public:
    cargoBoxInSpace(spriteClass* spr);
    void draw();
};

#endif // CARGOBOX_HPP_INCLUDED
