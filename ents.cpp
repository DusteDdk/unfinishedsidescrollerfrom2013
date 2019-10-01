#include "ents.hpp"

void gameObject::draw()
{
  if(sprite)
    sprite->draw();
}

gameObject::gameObject()
{
  sprite=NULL;
  bMan=NULL;
  die=0;
  velocity.x=0;
  velocity.y=0;
  lastPos.x=0;
  lastPos.y=0;
  team=projTeamEnemy; //Pr default
}

gameObject::~gameObject()
{
//  if(sprite)
//    delete sprite;
}


void gameObject::sim()
{
  if(velocity.x || velocity.y)
  {
    p.x +=velocity.x;
    p.y +=velocity.y;
    position(p);
  }
}

void gameObject::position(GLfloat x, GLfloat y)
{
  fdot p;
  p.x = x;
  p.y = y;
  position( p );
}

void gameObject::position( fdot pp )
{
  lastPos=p; //Update last position before updating current position
  p=pp;
  sprite->position(p);
}

void gameObject::damage( int points )
{
  health -= points;
  if(health <= 0)
    die=1;
}

//Checks for collision against this objects sprites and another (otherSprite)
bool gameObject::collide(spriteClass* otherSprite, collisionDetectionClass* cc, colinfo* ci, int dmg)
{
  if(sprite->hasCollisionLines())
  {
    if(cc->sprites( otherSprite,sprite, ci ))
    {
      ci->shealth=health;
      damage(dmg); //Apply the damage
      return(true);
    }
  }
  return(false);
}
