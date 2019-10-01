#ifndef ENTS_HPP_INCLUDED
#define ENTS_HPP_INCLUDED

#include "sprite.hpp"
#include "control.hpp"
#include "camcontrol.hpp"
#include "collisiontest.hpp"
#include "ents/bullets.hpp"

class controlClass;
class cameraControlClass;

enum objTypes { objTile, objPlayerShip, objCamLimitTop, objCamLimitBottom, objFx, objAstroid0, objAstroid1, objAstroid2,
objCargoBox, objNukeFlying, objAstroid3,objShieldPower, objRepairKit, objCutter1, objErr };

//Gameobject properties (from the lvl file)
struct goProperties
{
  //Don't know how these are going to work yet..
  /*int ai;
  int behaviour;*/
  bool spawnbehind; //Entity will spawn after just after the camera passed it (come in from behind)

  //Done (in editor)
  int hiTag, loTag; //Yes, I was inspired by Duke Nukem 3D, sorry.
  bool enemy; //Recieves fire //TODO: Deprecate, use team instead
  bool wait; //Ship will stop flying untill this is killed
  bool boss; //Level is completed when killed
  bool mustLive; //gameover if dies

  //Done (in editor, and game)
};

//All drawn objects
class gameObject {
  public:
    gameObject();
    ~gameObject(); //Objects themselves are responsible for cleaning up after themselves.
    goProperties props;
    int layer; // which layer is this on?
    bool die; //Kill next frame

    int type; //Kind of object 0=tile, rest is objects
    string strName; //String name (only used in editor)
    int health; //Health left
    int colDmg; //damagepoints delt to objects that collide with this
    fdot accelSpeed; //acceleration
    fdot maxSpeed; //Max speed
    fdot cruiseSpeed; //Cruising speed
    int movePattern; //moving behaviour
    int shootPattern; //shooting behaviour
    int introPattern; //intro behaviour (0=none)
    enum projectileTeam team; //Which team

    fdot p; //position
    fdot lastPos; //Position last frame

    bool left; //moving from left to right instead of opposite HMMM???
    fdot velocity;
    spriteClass* sprite; //Main sprite of all drawn objects
    bulletManagerClass* bMan; //Bullet manager
    virtual void draw(); //Draw this object
    virtual void sim(); //Move/Think (not all objects will do this)
    virtual void position(GLfloat x,GLfloat y); //move to position
    virtual void position( fdot pp );

    virtual void damage( int points );
    virtual bool collide(spriteClass* otherSprite, collisionDetectionClass* cc, colinfo* ci, int dmg); //Collision against objects

};

#endif // ENTS_HPP_INCLUDED
