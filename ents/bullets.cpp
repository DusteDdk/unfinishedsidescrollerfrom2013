#include "bullets.hpp"

bulletManagerClass::bulletManagerClass(spriteManagerClass* s)
{
  sprMan=s;
  defaultSpriteId = sprMan->load( "/gfx/weapons/proj.png" );
}

void bulletManagerClass::spawn(fdot sPos, fdot dir, enum ammoType a, enum projectileTeam team)
{
  ammoClass tempBullet;

  tempBullet.ammoType = a;
  tempBullet.team = team;
  tempBullet.sprite = sprMan->spawn(defaultSpriteId);
  tempBullet.spos=sPos;
  tempBullet.pos=sPos;
  tempBullet.vel = dir;
  tempBullet.vel.x *=50;

  bullets.push_back(tempBullet);
}

void bulletManagerClass::draw()
{
  for(int i=0; i < bullets.size(); i++)
  {
    bullets[i].draw();
  }
}

void bulletManagerClass::sim(GLfloat xlim)
{
  for(vector<ammoClass>::iterator it=bullets.begin();it != bullets.end(); ++it)
  {
    if(it->pos.x > xlim)
    {
      it->dead=1;
    }
    if(it->dead)
    {
      it=bullets.erase(it);
      if(it==bullets.end()) break;
    } else {
      it->sim();
    }
  }
}
