#include "sprite.hpp"
#include "control.hpp"

#ifdef WIN32
  typedef unsigned int uint;
#endif

#ifndef buttonBaseHeader
#define buttonBaseHeader

//#define DEBUG_CLICKABLE_AREA
class buttonBase {
  private:
  void *caller;
  void (*fp)(void *c);
  int t; //index of current texture
  spriteClass* tex[3]; //idle, hover, click;
  int geomX[2], geomY[2];
  public:
  void check(inputClass *inp);
  buttonBase(void(*f)(void* c),void* cal, GLfloat posx, GLfloat posy );
  void texture(string idleFile, string hoverFile, string clickFile, spriteManagerClass* sprMan);
  GLfloat px,py;
  void click(bool c);
  void hover();
  void draw();
  bool mousedown;
  void setGeometry(int xa,int  ya,int  xb,int  yb); //Geometry of the clickable area
};

#endif
