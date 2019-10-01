
#ifndef CHECKHEADER
#define CHECKHEADER

#include <iostream>
#include "sprite.hpp"
#include "control.hpp"

using namespace std;

class checkBoxClass {
  private:
    inputClass* inp;
    bool mousedown;
    bool callBack;
    void *caller;
    void (*fp)(void *c);

  public:
    spriteClass* sprite[2];
    checkBoxClass( int sprId[2], inputClass* in, bool state, spriteManagerClass* s );
    void setPos( GLfloat x, GLfloat y );
    bool checked;
    void draw();
    void setCallBack(void(*f)(void *c),void* cal);
    GLfloat px,py;
};

#endif
