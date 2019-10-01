#ifndef WEAPON_HPP_INCLUDED
#define WEAPON_HPP_INCLUDED

#include "../sprite.hpp"
#include "ammo.hpp"
#include "../general.hpp"
#include "bullets.hpp"

class weaponClass
{
  public:
    //Ammo type, number of rounds fired pr shot, rate of fire (shots pr second), direction (0 = -> 90 | 180 <-), can this weapon aim?, if yes how much.
    weaponClass(bulletManagerClass* bm, enum ammoType a, float shotsPrSec, fdot d);
    void setBarrelSprite(string spriteFile, spriteManagerClass* sprMan);
    void setBaseSprite(string spriteFile, spriteManagerClass* sprMan);
    void setOffset(GLfloat x, GLfloat y);
    void setMuzzPos(GLfloat x, GLfloat y); //Relative to mainSprite
    void draw(GLfloat x, GLfloat y);
    int update(fdot pos, bool trigger); //Return the number of projectiles fired
    spriteClass* mainSprite;
    spriteClass* muzzleFlash;
    projectileTeam team; //0= player, 1=enemy

  private:
    bulletManagerClass* bMan; //To spawn bullets
    fdot muzPos;
    enum ammoType ammoType;
    GLfloat px,py; //Relative to the ship
    int delay; //Delay between each shot
    int timeLeft; //Time left before shooting
    fdot dir;  //Normalized direction
    bool shooting; //Is weapon playing/doing shooting?
};


#endif // WEAPON_HPP_INCLUDED
