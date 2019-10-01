#ifndef HUD_HPP_INCLUDED
#define HUD_HPP_INCLUDED

#include "gameinfo.hpp"
#include "sprite.hpp"
#include "scene.hpp"
#include "text.hpp"

class hudInventoryBox;

struct hudMsg
{
  string msg;
  int ms;
};

class hudClass
{
  public:
    hudClass(gameInfo_t *gi,spriteManagerClass* sprMan, glTextClass* tc);
    void draw();
    void addHudMsg(string msg, int ms);

  private:
    glTextClass* glText;
    spriteClass *mainSprite, *batterySprite, *healthSprite;
    spriteClass *endGlow; //the glow in the end of the liquid
    gameInfo_t* gameInfo;
    hudInventoryBox* inv[5];
    GLfloat shownHealth,shownBattery;
    void drawReverseProgressBar(GLfloat x, GLfloat y, GLfloat len, spriteClass* spr, GLfloat percentage);
    vector<hudMsg> msgs;
    int msMsgShown;
};

class hudInventoryBox
{
  public:
    hudInventoryBox(int slot, spriteClass* hudSpr, spriteManagerClass* sprMan);
    void draw();
  private:
    spriteClass* sprite;
    spriteClass* content;
};

#endif // HUD_HPP_INCLUDED
