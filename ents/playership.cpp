#include "playership.hpp"

extern int ticks; //This contains the number of MS since last call

playerShip::playerShip()
{
  weapTop=NULL;
  weapFront=NULL;
  weapBot=NULL;
  engineSprite=NULL;
}

void playerShip::draw()
{
 // printc("Playership (%i) at (%.2f, %.2f)", this, p.x, p.y);
  sprite->draw();
  if(weapTop)
    weapTop->draw(p.x,p.y);
  if(weapFront)
    weapFront->draw(p.x,p.y);
  if(weapBot)
    weapBot->draw(p.x,p.y);

  if(engineSprite)
  {
    engineSprite->position( p.x+2-engineSprite->size.x, p.y+7 );
    engineSprite->draw();
  }
}

void playerShip::sim()
{
  //Dampen
  if(velocity.y < 0.0)
  {
    velocity.y -= velocity.y/14.0;
    if(velocity.y > 0.0)
      velocity.y=0;
  }

  if(velocity.y > 0.0)
  {
    velocity.y -= velocity.y/14.0;
    if(velocity.y < 0.0)
      velocity.y=0;
  }
//x is dampened by camcontrol

  if(inp->up)
  {
    velocity.y -= 1.0;
  } else if(inp->down)
  {
    velocity.y += 1.0;
  }
  if(inp->left)
  {
    velocity.x -= 1;
  } else if(inp->right)
  {
    velocity.x += 1;
  }


  //Max speed (should be taken from attached engine)
  if(velocity.x < -10.0)
  {
    velocity.x = -10.0;
  }
  if(velocity.x > 14)
  {
    velocity.x=14;
  }


  p.x +=velocity.x;
  p.y +=velocity.y;
  position(p);

  //Shoot?
  //Fire top mount
  if(this->weapTop)
  {
    pi->shotsFired += weapTop->update(this->p,inp->shoot);
  }
  //Fire front mount
  if(this->weapFront)
  {
    pi->shotsFired += weapFront->update(this->p,inp->shoot);
  }
  //Fire bot mount
  if(this->weapBot)
  {
    pi->shotsFired += weapBot->update(this->p,inp->shoot);
  }

}


void playerShip::setup(spriteManagerClass* sprMan, playerInfo* info)
{
  //Setup hull
  shield=0.0;
  pi=info;

  switch((int)info->shipHull)
  {
    case 1: //Case 1: Standard hull.
      //sprite = sprMan->spawnByFile( "gfx/testhull.tex" );
      healthMax=1000;
      batteryMax=1000;
    break;
    case 2:
      sprite = sprMan->spawnByFile( "gfx/testhull2.png" );
      healthMax=1500;
      batteryMax=1000;
    break;
  }

  //Make sure theres collines
  if(!sprite->hasCollisionLines())
  {
    sprMan->genCollisionLines( sprite->getId() );
  }

  setHealthPoints(healthMax);
  setBatteryPoints(batteryMax);

  fdot aimDir;
  aimDir.x=1;
  aimDir.y=0;
  //Connect engine
  switch((int)info->shipSlot[slotEngine])
  {
    case 1: //Mount default engine
      engineSprite=sprMan->spawnByFile("gfx/ship/engine1.tex");
    break;
  }
  //Mount Top Weapon
  switch((int)info->shipSlot[slotTop])
  {
    case 1:
      weapTop = new weaponClass( bMan,ammoSmallBullet, 25 ,aimDir );
      weapTop->mainSprite = sprMan->spawnByFile("gfx/weapons/topgatling.tex");
      weapTop->setOffset(17,-17);
      weapTop->muzzleFlash = sprMan->spawnByFile("gfx/weapons/muzgat.tex");
      weapTop->setMuzzPos( 80,11 );
      weapTop->team=team;
    break;
  }
  //Mount Front Weapon
  switch((int)info->shipSlot[slotFront])
  {
    case 1:
      weapFront = new weaponClass( bMan,ammoSmallBullet, 4, aimDir );
      weapFront->mainSprite = sprMan->spawnByFile("gfx/weapons/frontgatling.tex");
      weapFront->setOffset( 110, 18 );
      weapFront->muzzleFlash = sprMan->spawnByFile("gfx/weapons/muzgat.tex");
      weapFront->setMuzzPos( 30,11 );
      weapFront->team=team;
    break;
  }

  //Mount Bottom Weapon
  switch((int)info->shipSlot[slotBottom])
  {
    case 1:
      weapBot = new weaponClass( bMan,ammoSmallBullet, 25,aimDir );
      weapBot->mainSprite = sprMan->spawnByFile("gfx/weapons/gatling2.tex");
      weapBot->setOffset(19,45);
      weapBot->team=team;
    break;
    case 2:
      weapBot = new weaponClass( bMan,ammoBigBullet, 5, aimDir );
      weapBot->mainSprite = sprMan->spawnByFile("gfx/weapons/gatling1.tex");
      weapBot->setOffset(19,45);
      weapBot->muzzleFlash = sprMan->spawnByFile("gfx/weapons/muzgat.tex");
      weapBot->setMuzzPos( 80,11 );
      weapBot->team=team;
    break;
    case 3:
      weapBot = new weaponClass( bMan,ammoRocket, 1.0, aimDir);
      weapBot->mainSprite = sprMan->spawnByFile("gfx/weapons/missilebox1.tex");
      weapBot->setOffset(19,45);
      weapBot->team=team;
    break;
  }

  cout << "ShipSetup run for: " << info->shipName << endl;
  cout << "  Hull   : " << (int)info->shipHull << endl;
  cout << "  Engine : " << (int)info->shipSlot[slotEngine] << endl;
  cout << "  Top    : " << (int)info->shipSlot[slotTop] << endl;
  cout << "  Front  : " << (int)info->shipSlot[slotFront] << endl;
  cout << "  Bottom : " << (int)info->shipSlot[slotBottom] << endl;

  for(int i=slotCargo1; i < slotCargo5+1; i++)
  {
    cout << "  Cargo "<<i<<" : " << (int)info->shipSlot[i] << endl;
  }
}

void playerShip::setHealthPoints(int points)
{
  if(points > healthMax)
    health=healthMax;
  else
    health=points;

  //Update percentage
  healthPercentage=100.0/(float)healthMax*(float)health;
}

void playerShip::setBatteryPoints(int points)
{
  //Update percentage
  if(points > batteryMax)
    battery=batteryMax;
  else
    battery=points;
  //Update percentage
  batteryPercentage=100.0/(float)batteryMax*(float)battery;
}

void playerShip::damage( int points )
{
  //TODO: extract from battery if shield
  int left=points;
  left -= battery;   // *shield effectivity
  battery -= points; // /shield effectivity
  if(battery < 0) battery=0;
  setBatteryPoints(battery);

  if(left>0)
  {
    health -= left;
    if(health <0)
    {
      health=0;
      //die();
    }
    setHealthPoints( health );
  }

}
