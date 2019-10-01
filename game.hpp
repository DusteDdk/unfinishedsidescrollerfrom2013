#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include <vector>

#include "glfx.hpp"

#include "level.hpp"
#include "text.hpp"
#include "control.hpp"
#include "sprite.hpp"
#include "playerprofile.hpp"
#include "settings.hpp"
#include "scene.hpp"
#include "camcontrol.hpp"
#include "missioninfo.hpp"
#include "hud.hpp"
#include "gameinfo.hpp"
#include "console.hpp"
#include "terrain.hpp"
#include "collisiontest.hpp"
#include "effectqueue.hpp"
#include "ents/bullets.hpp"

class gameClass
{
  public:
    gameClass(spriteManagerClass* s, glTextClass* g, inputClass* i, gameInfo_t* gi);
    ~gameClass();
    bool run();
  private:
    spriteClass* vagina;

    spriteClass* pauseSprite;
    gameInfo_t* gameInfo;
    spriteManagerClass* sprMan;
    glTextClass* glText;
    inputClass* inp;
    hudClass* hud;
    bulletManagerClass* bulletManager;
    mapClass* map; //The level that is loaded
    terrainClass* terr;
    vector<gameObject*> layer[4]; //Graphical objects
    cameraControlClass* camControl;
    collisionDetectionClass* colDetection;
    effectQueueClass* fxQueue;
    GLfloat pOffset[4]; //offsets
};
#endif // GAME_HPP_INCLUDED
