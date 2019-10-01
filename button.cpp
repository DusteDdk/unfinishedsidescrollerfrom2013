#include "button.hpp"
#include "control.hpp"
#include "dirs.hpp"
extern dirsClass dir;


buttonBase::buttonBase(void(*f)(void *c),void* cal, GLfloat posx, GLfloat posy)
{
  geomX[0] = 0;
  geomX[1] = 0;
  geomY[0] = 0;
  geomY[1] = 0;

  fp=f;
  caller=cal;

  px=posx;
  py=posy;

  mousedown=0;

  t=0;
}

void buttonBase::setGeometry(int xa,int  ya,int  xb,int  yb)
{
  geomX[0] = xa;
  geomX[1] = xb;
  geomY[0] = ya;
  geomY[1] = yb;
}

void buttonBase::check(inputClass *inp)
{
  //To debug clickable area
  #ifdef DEBUG_CLICKABLE_AREA
  glDisable( GL_TEXTURE_2D );
  glColor4f( 1,0,0,0.7);
  glBegin( GL_QUADS );
    glVertex2f( px+geomX[0], py+geomY[0] );
    glVertex2f( px+tex[0]->width-geomX[1], py+geomY[0] );
    glVertex2f( px+tex[0]->width-geomX[1], py+tex[0]->height-geomY[1] );
    glVertex2f( px+geomX[0], py+tex[0]->height-geomY[1] );
  glEnd( );
  glEnable( GL_TEXTURE_2D );
  #endif

  if(inp->mousex > px+geomX[0] && inp->mousex < px+tex[0]->size.x-geomX[1] && inp->mousey > py+geomY[0] && inp->mousey < py+tex[0]->size.y-geomY[1])
  {
    if(inp->mousedown || mousedown)
    {
      click(inp->mousedown);
    } else {
      hover();
    }
  } else {
    mousedown=0;
    t=0;
  }
}

void buttonBase::click(bool c)
{
  //play ani
  if(!mousedown)
  {
    t=2;
    tex[t]->stop();
    tex[t]->play(0,0,0);
  }
  //Wait for release and for the animation to play through
  mousedown=1;
  if(!c && mousedown && !tex[t]->playing)
  {
    mousedown=0;
    fp(caller);
  }
}

void buttonBase::hover()
{
  if(t != 1 && t!= 2)
  {
    t=1;
    tex[t]->play(1,0,0);
  }

  if(t==2)
  {
    //Wait for animation to finish.
    if(!tex[t]->playing)
    {
      t=1;
      tex[t]->play(1,0,0);
    }
  }
}

void buttonBase::draw()
{
  tex[t]->draw();
}

void buttonBase::texture(string idleFile, string hoverFile, string clickFile, spriteManagerClass* sprMan)
{
  tex[0] = sprMan->spawn( sprMan->load( "gfx/menu/buttons/"+idleFile ) ) ;
  tex[1] = sprMan->spawn( sprMan->load( "gfx/menu/buttons/"+hoverFile ) ) ;
  tex[2] = sprMan->spawn( sprMan->load( "gfx/menu/buttons/"+clickFile ) ) ;

  tex[0]->position( px, py );
  tex[1]->position( px, py );
  tex[2]->position( px, py );
}

///TODO: Destructor for button (delete sprite)
