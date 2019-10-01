#include "game.hpp"

extern settingsClass set;
extern struct sceneInfo_t sceneInfo;
extern playerClass player;
extern glFxClass *glFx;
extern int ticks;

gameClass::gameClass(spriteManagerClass* s, glTextClass* g, inputClass* i, gameInfo_t* gi)
{
  sprMan=new spriteManagerClass(); //s;
  glText=g;
  inp=i;
  gameInfo=gi;
  //TODO: These have to be updated when the level ends (score should only be updated if success)
  gameInfo->score=0;
  gameInfo->credits=gameInfo->player->credits;
  hud = new hudClass(gameInfo, sprMan, glText);
  bulletManager=new bulletManagerClass(sprMan);
  //Load map
  map = new mapClass(sprMan);
  map->bMan=bulletManager; //Set bullet manager, needed by the spawner

  if(!map->loadMap( gameInfo->mission->missionFile, false ))
  {
    printc("Game: loadMap failed, Couldn't load '%s'", gameInfo->mission->missionFile.data() );
    inp->keyStates[SDLK_ESCAPE]=1;
  }
  pOffset[0] = map->pOffset[0];
  pOffset[1] = map->pOffset[1];
  pOffset[2] = 1.0;
  pOffset[3] = 1.0;
  sceneInfo.camX=0;
  sceneInfo.camY=0;
  camControl = new cameraControlClass();

  //Add terrain
  terr = new terrainClass( map->lines );

  //Effect queue
  fxQueue=new effectQueueClass();

  //Add stuff
  for(vector<gameObject*>::iterator it=map->objects.begin(); it!=map->objects.end(); ++it)
  {
    //Add camera limiters
    if((*it)->type==objCamLimitBottom || (*it)->type==objCamLimitTop)
    {
      camControl->addPoint((*it));
      map->objects.erase(it);
      it=map->objects.begin();
    }

    if((*it)->type==objFx)
    {
      fxQueue->queue( (*it)->p, (*it)->props.hiTag, (*it)->props.loTag );
      map->objects.erase(it);
      it=map->objects.begin();
    }
  }


  //Generate collision lines for everything except bg
  for(int i=0; i<map->objects.size(); i++)
  {
    if( map->objects.at(i)->type > 0)
    {
      /*if(map->objects.at(i)->sprite)
        sprMan->genCollisionLines( map->objects.at(i)->sprite->getId() );*/

      //Add input class to ship
      if( map->objects.at(i)->type==objPlayerShip)
      {
        gameInfo->ship=static_cast<playerShip*>( map->objects.at(i) );
        gameInfo->ship->inp=inp;
        gameInfo->ship->team = projTeamPlayer;
        gameInfo->ship->setup(sprMan,  gameInfo->player);
        sceneInfo.camX = gameInfo->ship->p.x;
        sceneInfo.camY = gameInfo->ship->p.y;
       // sprMan->genCollisionLines( player->sprite->getId() );
      }
    }
  }

  //Setup effectQueue now that we have the ship and stuff.
  fxQueue->setup( camControl, &sceneInfo );

  //Collision detection
  colDetection=new collisionDetectionClass();

//  camControl->setSpeed(20.0);
  pauseSprite = sprMan->spawnByFile( "gfx/hud/pause.png" );
  pauseSprite->position( sceneInfo.X/2.0-pauseSprite->halfSize.x, sceneInfo.Y/2.0-pauseSprite->halfSize.y );

  vagina = sprMan->spawnByFile("/gfx/weapons/expl0.tex");
}

gameClass::~gameClass()
{
  printc(" ");
  printc("Game Class Cleaning up:");
  printc("Deleting map.");
  delete map;
  printc("Deleting camcontrol..");
  delete camControl;
  printc("Deleting terrain..");
  delete terr;
  printc("Deleting object lists..");
  //Clear the vectors
  for(int i=0; i < 4; i++)
  {
    layer[i].clear();
  }
  //Delete vector of specials
  printc("Deleting spritemanager..");
  delete sprMan;

  printc("Deleting collision detection class..");
  delete colDetection;

  printc("Deleting effectQueue..");
  delete fxQueue;

  printc("Gameclass shutdown.");
  printc(" ");
}

bool gameClass::run()
{
  //Update time played.
  static int ms = 0;
  ms += ticks;
  if(ms >= 1000)
  {
    ms=0;
    gameInfo->player->timePlayed++;
  }
  //Add gameobjects to layer vectors as they become relevant
  //TODO: use pOffset to decide (is that possible?)
  vector<gameObject*>::iterator it=map->objects.begin();
  while( (it!=map->objects.end()) && ((*it)->p.x < sceneInfo.X+sceneInfo.camX) )
  {
 //   printc("Adding type: %i to layer %i at (%i,%i)", (*it)->type, (*it)->layer, (int)(*it)->p.x,(int)(*it)->p.y);
    if((*it)->layer >= 0 && (*it)->layer < 3)
    {
      layer[(*it)->layer].push_back( (*it) );
    } else if((*it)->layer == 3)
    {
      //special objects are in layer 3 check for those
      switch((*it)->type)
      {
        default:
          layer[3].push_back( (*it) );
        break;
      }
    }

    //Remove from vector
    map->objects.erase(it);
    it=map->objects.begin();
  }

  //Things that are pause sensitive
  if(!inp->paused)
  {
    //Update camera
    camControl->update(gameInfo->ship);

    //Update terrain
    terr->update(sceneInfo.camX);


    //Sim
    for(int l=0; l < 4; l++)
    {
      for(vector<gameObject*>::iterator it=layer[l].begin(); it!=layer[l].end(); ++it)
      {
        //Check if this object should die.
        if((*it)->die)
        {
          it = layer[l].erase( it );
          if(it == layer[l].end())
          {
            break;
          }
        } else {
          if((*it)->type > 0)
          {
            //Simulate
            (*it)->sim();
          }
        }
      }
    }
    bulletManager->sim(sceneInfo.X + sceneInfo.camX);

    //Collision between ship and env
    colinfo ci;
    //gameInfo->ship->sprite->updateCollisionLines();
   /* if( terr->collide( gameInfo->ship->sprite->collisionLines, &ci ) )
    {
      //Position ship where it no longer collides (it's last position)
      gameInfo->ship->position( gameInfo->ship->lastPos );
      //Bounce back a bit
      gameInfo->ship->velocity.x = ci.dir.x*1.5;
      gameInfo->ship->velocity.y = ci.dir.y*1.5;
      printc("Dir: %.2f , %.2f",ci.dir.x, ci.dir.y);
      ///TODO: Hitsound
      ///TODO: Effect
      //Ship takes damage
      gameInfo->ship->damage( 100 );
    }*/

    //Collision between objects and ship
  /*  for(int i=0; i < layer[2].size(); i++)
    {
      if(layer[2][i]->type!=objPlayerShip)
      {
        if( (*layer[2][i]).collide( gameInfo->ship->sprite, colDetection, &ci, gameInfo->ship->colDmg ) )
        {
          switch( (*layer[2][i]).type )
          {
            case objShieldPower:
              gameInfo->ship->setBatteryPoints( gameInfo->ship->battery + 250 );
              hud->addHudMsg("Battery!", 500);
              (*layer[2][i]).die=1;
              printc("TODO: Power pickup effect.");
            break;
            case objCargoBox:
              printc("TODO: Cargo effect, add cargo");
              hud->addHudMsg("Random", 500);
              (*layer[2][i]).die=1;
            break;
            break;
            case objAstroid0:
            case objAstroid1:
            case objAstroid2:
            case objAstroid3:
              printc("TODO: Spawn rock+metal debris, dust and sound.");
              gameInfo->ship->velocity.x = ci.dir.x*-1.5;
              gameInfo->ship->velocity.y = ci.dir.y*1.5;

            break;
            case objRepairKit:
              gameInfo->ship->setHealthPoints( gameInfo->ship->health + 250 );
              (*layer[2][i]).die=1;
              hud->addHudMsg("Repair!", 500);
              printc("TODO: Repair pickup effect.");
            break;
            default: //Collision between monster and ship
              printc("Unhandled collision %i", (*layer[2][i]).type);
            break;
          } //switch type

          gameInfo->ship->damage( (*layer[2][i]).colDmg ); //deal damage to ship
          //Only bounce back if object still alive
          if( !(*layer[2][i]).die )
          {
            gameInfo->ship->position( gameInfo->ship->p.x + 5.0*-ci.dir.x, gameInfo->ship->p.y +5.0*ci.dir.y );
          }

        } //Collision true
      }
    }*/

    //Collision between objects??

    //Rotation (hack)
    if(gameInfo->ship)
    {
      if(inp->up)
      {
        gameInfo->ship->sprite->rotate( -45 );
      } else if(inp->down)
      {
        gameInfo->ship->sprite->rotate( 45 );

      } else {
        gameInfo->ship->sprite->rotate( 0 );
      }
    }

    //Bullets (demo hack)
    for(vector<gameObject*>::iterator it=layer[2].begin(); it!=layer[2].end(); ++it)
    {
      for(int i=0; i < bulletManager->bullets.size(); i++)
      {
        if(bulletManager->bullets[i].team != (*it)->team)
        {
          if(this->colDetection->box(bulletManager->bullets[i].sprite, (*it)->sprite))
          {
            bulletManager->bullets[i].dead=1;

            if( (*it)->type != objTile )
            {
              (*it)->damage( 20 );
              if( (*it)->die )
              {
                gameInfo->player->enemiesKilled++;
                vagina->position( (*it)->sprite->pos.x + (*it)->sprite->halfSize.x - vagina->halfSize.x,
                (*it)->sprite->pos.y + (*it)->sprite->halfSize.y-vagina->halfSize.y );
                vagina->play(0,0,0);
              }
            }//not a tile (specials are not in layer 2)
          }
        }
      }
    }

    fxQueue->run();

  } //Pause sensitive (not paused)

  //draw
  glColor4f(1,1,1,1);
  for(int l=0; l < 4; l++)
  {
    glPushMatrix();
    glTranslatef( -sceneInfo.camX*pOffset[l], -sceneInfo.camY*pOffset[l], 0 );

    if(l==2)
    {
      bulletManager->draw();
    if(vagina->playing)
      vagina->draw();

    }

    for(vector<gameObject*>::iterator it=layer[l].begin(); it!=layer[l].end(); ++it)
    {
      //In screen?
      if((*it)->p.x+512 > sceneInfo.camX*pOffset[l] && (*it)->p.x*pOffset[l] < (sceneInfo.X+sceneInfo.camX))
      {
        (*it)->draw();
        ///DEBUG: Show sprite collision lines
        if( (*it)->sprite && (*it)->sprite->hasCollisionLines() )
        {
     //    (*it)->sprite->updateCollisionLines(); //Takes some cpu
         (*it)->sprite->drawCollisionLines(); //Takes a bit CPU
        }
      }
      //Out of the screen??
      if((*it)->p.x+512 < sceneInfo.camX*pOffset[l])
      {
        //Delete object
        delete (*it);
        vector<gameObject*>::iterator itb = layer[l].erase(it);
        if(itb==layer[l].end())
        {
          break;
        } else {
          it=itb;
        }
      }
    }

    ///DEBUG: Show cameraControl lines
   /* if(l==3)
    {
      camControl->debug();
    }*/
    ///DEBUG: Show terrain collision lines
    /*if(l==2)
    {
      terr->showColLines();
    }*/
    glPopMatrix();
  }




  ///Draw the HUD
  hud->draw();

  ///Show paused screen
  static GLfloat fadeBlack=0.0;
  static bool paused=0, unpausing=0;

  if(inp->paused && !unpausing)
  {
    paused=1;
    if(fadeBlack < 1)
      fadeBlack+= 1.0/15.0;
    glFx->blackwhite(fadeBlack);
    glFx->dirBlur(135, 10,2);
    glFx->dirBlur(45, 10,2);

    pauseSprite->draw();
  } else {
    if(paused && !unpausing)
    {
      unpausing=1;
      inp->paused=1;
    }
    if(unpausing)
    {
      if(fadeBlack > 0)
      {
        glFx->dirBlur(135, 10*fadeBlack,2);
        glFx->dirBlur(45, 10*fadeBlack,2);

        fadeBlack -= 1.0/15.0;
        glFx->blackwhite(fadeBlack);
      } else {
        unpausing=0;
        inp->paused=0;
      }
    }
  }

  //Debug collines from map
 /* map->showColZone();
  glEnable(GL_TEXTURE_2D);
  glColor4f(1,1,1,1);*/


  if(inp->keyStates[SDLK_ESCAPE])
  {
    inp->paused=0;
    return(0);
  }

  return(1);
}
