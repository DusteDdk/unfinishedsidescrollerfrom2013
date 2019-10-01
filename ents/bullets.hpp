#ifndef BULLETS_HPP_INCLUDED
#define BULLETS_HPP_INCLUDED

#include <vector>
#include "../general.hpp"
#include "../sprite.hpp"
#include "ammo.hpp"

class bulletManagerClass
{
  public:
    bulletManagerClass(spriteManagerClass* s);
    void spawn(fdot sPos, fdot dir, enum ammoType a, enum projectileTeam team);
    void draw();
    void sim(GLfloat xlim);
    vector<ammoClass> bullets; //Bullets in the game

  private:
    spriteManagerClass* sprMan;
    int defaultSpriteId;
};

#endif // BULLETS_HPP_INCLUDED
