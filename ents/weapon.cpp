#include "weapon.hpp"

extern int ticks; //This contains the number of MS since last call

weaponClass::weaponClass(bulletManagerClass* bm,enum ammoType a, GLfloat shotsPrSec, fdot d)
{
  bMan=bm;
  if(shotsPrSec > 0.0) //avoid div by 0
    delay = (int)(1000.0/shotsPrSec);
  else
    delay=0;
  timeLeft=0;

  this->ammoType=a;

  dir=d;

  mainSprite=NULL;
  muzzleFlash=NULL;
  muzPos.x=0.0;
  muzPos.y=0.0;
  team=projTeamEnemy; //pr default,
}

void weaponClass::setOffset(GLfloat x, GLfloat y)
{
  px=x;
  py=y;
}

void weaponClass::setMuzzPos(GLfloat x, GLfloat y)
{
  muzPos.x=x;
  muzPos.y=y;
}

void weaponClass::draw(GLfloat x, GLfloat y)
{
  if(mainSprite)
  {
    mainSprite->position(x+px,y+py);
    mainSprite->draw();
    //If weapon sprite is animated:
    if(mainSprite->numFrames>1)
    {
      //Stop when played through the sequence, (sets current frame to 0)
      if(!mainSprite->playing)
      {
        mainSprite->stop();
      }
      //If trigger is pressed, don't let it show frame 0
      if(mainSprite->frame==0 && shooting)
      {
        mainSprite->play(0,0,1);
      }
    }

  }

  if(muzzleFlash)
  {
    if(shooting && !muzzleFlash->playing)
    {
      muzzleFlash->play(0,0,0);
    }

    if(muzzleFlash->playing)
    {
      muzzleFlash->position(x+px+muzPos.x, y+py+muzPos.y-muzzleFlash->halfSize.y);
      muzzleFlash->draw();
    }
  }

    glDisable(GL_TEXTURE_2D);
    glColor4f(1,0,0,0.5);
    glBegin(GL_LINES);
      glVertex2f( x+px+muzPos.x, y+py+muzPos.y );
      glVertex2f( x+px+muzPos.x+100, y+py+muzPos.y );
    glEnd();
    glEnable(GL_TEXTURE_2D);
    glColor4f(1,1,1,1);

}

int weaponClass::update(fdot pos, bool trigger)
{
  fdot spawnPos;
  fdot dir;
  dir.x=1; dir.y=0; //Initial dir

  if(timeLeft)
  {
    timeLeft -= ticks;
    if(timeLeft < 0)
    {
      timeLeft=0;
    }
  }

  shooting=trigger;

  if(!timeLeft && trigger)
  {
    timeLeft=delay;
    spawnPos.x=pos.x+px+muzPos.x;
    spawnPos.y=pos.y+py+muzPos.y;

    bMan->spawn(spawnPos, dir, ammoType, team);
    return(1);
  }


  return(0);
}
