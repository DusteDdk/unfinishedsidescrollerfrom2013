#include "checkbox.hpp"

checkBoxClass::checkBoxClass( int sprId[2], inputClass* in, bool state, spriteManagerClass* s )
{
  callBack=0; //Pr default, no callback occurs when a checkbox changes state..
  mousedown=0;
  inp = in;
  sprite[0]=s->spawn(sprId[0]);
  sprite[1]=s->spawn(sprId[1]);

  //First sprite is off, not animated, second one is, start at a random frame

  checked=state;
}

void checkBoxClass::setPos( GLfloat x, GLfloat y )
{
  px=x;
  py=y;
  sprite[0]->position( px, py );
  sprite[1]->position( px, py );
}

void checkBoxClass::draw()
{
  if(inp->mousex  > px && inp->mousex < px+sprite[0]->size.x && inp->mousey > py && inp->mousey < py+sprite[0]->size.y)
  {
    if(mousedown==1 && inp->mousedown==0)
    {
      checked ? checked=0 : checked=1;
      mousedown=0;
      //Callback?
      if(callBack)
      {
        fp(caller);
      }
    }
    inp->mousedown ? mousedown=1 : mousedown=0;
  }


  if(checked)
    sprite[1]->draw();
  else
    sprite[0]->draw();
/*
  glColor4f(1,1,1,1);
  glDisable( GL_TEXTURE_2D );
  glBegin( GL_QUADS );
    glVertex2f( px, py );
    glVertex2f( px+sprite[0]->width, py );
    glVertex2f( px+sprite[0]->width, py+sprite[0]->height );
    glVertex2f( px, py+sprite[0]->height );
  glEnd();
  glEnable( GL_TEXTURE_2D );
  cout << px << "," << py << " - " << sprite[0]->width << ", " << sprite[0]->width << endl;*/
}

void checkBoxClass::setCallBack(void(*f)(void *c),void* cal)
{
    fp=f;
    caller=cal;
    callBack=1;
}
