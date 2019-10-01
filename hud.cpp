#include "hud.hpp"

extern struct sceneInfo_t sceneInfo;
extern int ticks;


hudClass::hudClass(gameInfo_t *gi, spriteManagerClass* sprMan, glTextClass* tc)
{
  gameInfo=gi;
  glText = tc;
  mainSprite = sprMan->spawnByFile( "gfx/hud/hud.png" );
  mainSprite->position( 0, sceneInfo.Y-mainSprite->size.y);
  batterySprite = sprMan->spawnByFile( "gfx/hud/battery-full.png" );
  healthSprite = sprMan->spawnByFile( "gfx/hud/health-full.png" );
  endGlow = sprMan->spawnByFile( "gfx/hud/end.png" );

  for(int i=0; i <5; i++)
  {
    inv[i] = new hudInventoryBox(i, mainSprite,sprMan);
  }

  shownBattery=0;
  shownHealth=0;
  msMsgShown=0;
}

void hudClass::draw()
{
  for(int i=4; i >-1; i--)
  {
    inv[i]->draw();
  }
  mainSprite->draw();

  bool writeDmg=0;
  //Health
  if( shownHealth > gameInfo->ship->healthPercentage)
  {
    writeDmg=1;
    shownHealth -= (shownHealth-gameInfo->ship->healthPercentage)/8 + 0.1;
    if(shownHealth < gameInfo->ship->healthPercentage) shownHealth=gameInfo->ship->healthPercentage;
  }
  if(shownHealth < gameInfo->ship->healthPercentage)
  {
    shownHealth += (gameInfo->ship->healthPercentage-shownHealth)/8 + 0.1;
    if(shownHealth > gameInfo->ship->healthPercentage) shownHealth=gameInfo->ship->healthPercentage;
  }
  drawReverseProgressBar(mainSprite->pos.x-1+22+118, mainSprite->pos.y-1+15, 118, healthSprite, shownHealth);
  //battery
  if( shownBattery > gameInfo->ship->batteryPercentage)
  {
    shownBattery -= (shownBattery-gameInfo->ship->batteryPercentage)/8 + 0.1;
    if(shownBattery < gameInfo->ship->batteryPercentage) shownBattery=gameInfo->ship->batteryPercentage;
    writeDmg=1;
  }
  if(shownBattery < gameInfo->ship->batteryPercentage)
  {
    shownBattery += (gameInfo->ship->batteryPercentage-shownBattery)/8 + 0.1;
    if(shownBattery > gameInfo->ship->batteryPercentage) shownBattery=gameInfo->ship->batteryPercentage;
  }
  drawReverseProgressBar(mainSprite->pos.x-1+140, mainSprite->pos.y-1+25,118, batterySprite, shownBattery);

  if(shownHealth > 0)
  {

    endGlow->position( mainSprite->pos.x-1+22+118-((118/100.0)*shownHealth)-endGlow->size.x/2.0,mainSprite->pos.y-1+11 );
    endGlow->draw();
  }
  if(shownBattery > 0)
  {

    endGlow->position( mainSprite->pos.x-1+140-((118/100.0)*shownBattery)-endGlow->size.x/2.0,mainSprite->pos.y-1+21 );
    endGlow->draw();
  }

  if(writeDmg)
  {
    addHudMsg(" SYS:DMG!",250);
  }
  if(shownHealth ==0)
  {
    addHudMsg("SYS:ERR!",10000);
  }

  if(msgs.size() > 0)
  {
    msMsgShown += ticks;
    glColor4f(1,0,0,1);
    glText->write( msgs[0].msg.data(), FONT_BITMAP_15,mainSprite->pos.x-1+35,mainSprite->pos.y-1+38);
    glColor4f(1,1,1,1);
    if(msgs[0].ms <= msMsgShown)
    {
      msgs.erase( msgs.begin() );
      msMsgShown=0;
    }
  }

}

void hudClass::drawReverseProgressBar(GLfloat x, GLfloat y, GLfloat len, spriteClass* spr, GLfloat percentage)
{
  glBindTexture( GL_TEXTURE_2D, spr->tex() );
  glBegin( GL_QUADS );
    glTexCoord2f(1-percentage/100.0,0);
    glVertex2f( x-((len/100.0)*percentage),y );

    glTexCoord2f(1,0);
    glVertex2f( x, y );
    glTexCoord2f(1,1);
    glVertex2f( x, y+spr->size.y+2 );

    glTexCoord2f(1-percentage/100.0,1);
    glVertex2i( x-((len/100.0)*percentage), y+spr->size.y+2 );
  glEnd();
}

void hudClass::addHudMsg(string msg, int ms)
{
  hudMsg t;
  //Check if same msg just came in.
  if(msgs.size() > 0)
  {
    if(msgs.at( msgs.size()-1 ).msg==msg)
    {
      return;
    }
  }
  t.msg=msg;
  t.ms=ms;
  msgs.push_back(t);
}

hudInventoryBox::hudInventoryBox(int slot,spriteClass* hudSpr, spriteManagerClass* sprMan)
{
  switch(slot)
  {
    case 0:
      sprite=sprMan->spawnByFile( "gfx/hud/inv1.png" );
      sprite->position( hudSpr->pos.x+194+(39*slot),hudSpr->pos.y+19);
    break;
    case 1:
      sprite=sprMan->spawnByFile( "gfx/hud/inv2.png" );
      sprite->position( hudSpr->pos.x+194+(39*slot),hudSpr->pos.y+19);
    break;
    case 2:
      sprite=sprMan->spawnByFile( "gfx/hud/inv3.png" );
      sprite->position( hudSpr->pos.x+194+(39*slot),hudSpr->pos.y+19);
    break;
    case 3:
      sprite=sprMan->spawnByFile( "gfx/hud/inv4.png" );
      sprite->position( hudSpr->pos.x+194+(39*slot),hudSpr->pos.y+19);
    break;
    case 4:
      sprite=sprMan->spawnByFile( "gfx/hud/inv5.png" );
      sprite->position( hudSpr->pos.x+194+(39*slot),hudSpr->pos.y+19);
    break;

  }

}

void hudInventoryBox::draw()
{
  sprite->draw();
}


