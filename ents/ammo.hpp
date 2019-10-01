#ifndef AMMO_HPP_INCLUDED
#define AMMO_HPP_INCLUDED

#include "../sprite.hpp"
#include "../general.hpp"

enum ammoType { ammoSmallBullet, ammoBigBullet, ammoRocket, ammoLaser, ammoRailGun };
enum projectileTeam { projTeamEnemy, projTeamPlayer };

class ammoClass
{
  public:
    ammoClass();
    void draw();
    void sim();
    enum ammoType ammoType;
    enum projectileTeam team;
    spriteClass* sprite;
    fdot pos;
    fdot spos;
    fdot vel;
    bool dead;
};

#endif // AMMO_HPP_INCLUDED
