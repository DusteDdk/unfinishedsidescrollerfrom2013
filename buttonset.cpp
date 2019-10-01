#include "buttonset.hpp"

void buttonSet::add(void(*f)(void *c),void* cal, GLfloat posx, GLfloat posy )
{
  if(idleFile.length() < 2)
  {
    printc("Error: ButtonSet got no textures");
    return;
  }
  btns.push_back( buttonBase( f, cal, posx, posy ) );
  btns.back().texture(idleFile,hoverFile,clickFile, sprMan);
  btns.back().setGeometry(geomX[0], geomY[0], geomX[1], geomY[1]);

  geomX[0] = 0;
  geomX[1] = 0;
  geomY[0] = 0;
  geomY[1] = 0;
}

void buttonSet::geometry(int xa,int ya,int xb,int yb)
{
  geomX[0] = xa;
  geomX[1] = xb;
  geomY[0] = ya;
  geomY[1] = yb;
}


void buttonSet::init( spriteManagerClass* r )
{
  sprMan = r;
  geomX[0] = 0;
  geomX[1] = 0;
  geomY[0] = 0;
  geomY[1] = 0;

}

void buttonSet::texture(string i, string h, string c)
{
  idleFile=i;
  hoverFile=h;
  clickFile=c;
}

void buttonSet::run(inputClass* inp,bool check)
{
  for(vector<buttonBase>::iterator it = btns.begin() ; it != btns.end(); ++it)
  {
    if(check)
      it->check(inp);
    it->draw();
  }
}
