#include <vector>
#include "control.hpp"
#include "button.hpp"
#include "sprite.hpp"
#include "console.hpp"

using namespace std;

#ifndef buttonSetHeader
#define buttonSetHeader
class buttonSet {
  private:
  vector<buttonBase> btns; //A vector of buttons
  spriteManagerClass *sprMan;
  string idleFile,hoverFile,clickFile;
  int geomX[2], geomY[2];
  public:
    void run(inputClass* inp,bool check);
    void init(spriteManagerClass* r);
    void texture(string i, string h, string c);
    void geometry(int xa,int ya,int xb,int yb);
    void add(void(*f)(void *c),void* cal, GLfloat posx, GLfloat posy);

};
#endif
