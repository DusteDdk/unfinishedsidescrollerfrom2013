#include "editor.hpp"

extern dirsClass dir;
extern settingsClass set;
extern struct sceneInfo_t sceneInfo;

editorClass::editorClass( inputClass* input, glTextClass* glT )
{
  inp = input;
  inp->mousedown=0;
  inp->mouseclick=0;
  glText=glT;
  blockInp=0;
  printc("Editor inti");
  //Grab the mouse
 // #ifndef WIN32
    // SDL_WM_GrabInput(SDL_GRAB_ON);
 // #endif

  pOffset[0]=0.3; //Farthest background
  pOffset[1]=0.6; //Background 1
  pOffset[2]=1.0; // Because this is the "game layer"
  pOffset[3]=1.0; // These are the same and this is the "mask layer"
  //Setup tools

  resetInput();

  //First test object, the test hull
  shipSpriteId=sprMan.load( "gfx/testhull.tex" );

  help=0;
  //Help text.
  txthelp[0] = "[h]elp [s]nap [p]roperties [q]Previous [o]ptions [w]Next [1]Type";
  txthelp[1] = "Level editor controls:\nh = Toggle help.\nESC = Exit level editor.\ng = Toggle Grid on/off\ns = Toggle Snap To Grid.\n"
  "Left Mouse = Place object, or grab object.\nDEL = Delete grabbed object.\nb = Show bounding boxes on sprites.\np = Object Properties\n"
  "O = Map Options.\nc = show collision area\nx = show cameraguides\n1 = Cycle through Types: Tiles, Objects, Specials.\nq = Previous Object\nw = Next Object.\n"
  "l = Cycle through layers.\ny = Less Reduction.\nt = More Reduction.\n7 = pOffset0++\n8 = pOffset0--\n9 = pOffset1++\n0 = pOffset1--\n"
  "LCTRL+n = Clear Everything.\nCursor Keys: Move round\nYou can also move by putting the cursor to the edge of the screen.\n"
  "z = Fast Zoom (Zoom in direction of cursor)\nMouse Scroll: Zoom\n"
  "Home = Go to 0,0 (initial position)\n";
  txthelp[2] = "Bugs:\n"
  "When zooming , collision lines are not shown correctly disable them when zooming.\n"
  "When using snap to grid, zoom should be 1.0 \n";

  txtWorkTypeNames[0] = "Tiles";
  txtWorkTypeNames[1] = "Objects";
  txtWorkTypeNames[2] = "Special";

  //Setup interface objects
  int chkBoxSpriteId[2];
  chkBoxSpriteId[0] = sprMan.load( "gfx/menu/checkboxoff.tex" );
  chkBoxSpriteId[1] = sprMan.load( "gfx/menu/checkboxon.tex" );
  //Worklayer selection
  chkWorkLayer[0] = new checkBoxClass( chkBoxSpriteId, inp, 0, &sprMan );
  chkWorkLayer[0]->setPos( 5,10 );
  chkWorkLayer[0]->setCallBack( chkWorkLayerCall,this );
  chkWorkLayer[1] = new checkBoxClass( chkBoxSpriteId, inp, 0, &sprMan );
  chkWorkLayer[1]->setPos( 5,40 );
  chkWorkLayer[1]->setCallBack( chkWorkLayerCall,this );
  chkWorkLayer[2] = new checkBoxClass( chkBoxSpriteId, inp, 1, &sprMan ); //Layer 2 is the gamelayer, it's checked pr default
  chkWorkLayer[2]->setPos( 5,70 );
  chkWorkLayer[2]->setCallBack( chkWorkLayerCall,this );
  chkWorkLayer[3] = new checkBoxClass( chkBoxSpriteId, inp, 0, &sprMan );
  chkWorkLayer[3]->setPos( 5,100 );
  chkWorkLayer[3]->setCallBack( chkWorkLayerCall,this );

  for(int i=0; i < 4; i++)
    chkWorkLayerLast[i]=false;

  //which layers to show
  chkShowLayers[0] = new checkBoxClass( chkBoxSpriteId, inp, 1, &sprMan );
  chkShowLayers[0]->setPos( 35, 10 );
  chkShowLayers[0]->setCallBack( dummyCall,this );
  chkShowLayers[1] = new checkBoxClass( chkBoxSpriteId, inp, 1, &sprMan );
  chkShowLayers[1]->setPos( 35, 40 );
  chkShowLayers[1]->setCallBack( dummyCall,this );
  chkShowLayers[2] = new checkBoxClass( chkBoxSpriteId, inp, 1, &sprMan );
  chkShowLayers[2]->setPos( 35, 70 );
  chkShowLayers[2]->setCallBack( dummyCall,this );
  chkShowLayers[3] = new checkBoxClass( chkBoxSpriteId, inp, 1, &sprMan );
  chkShowLayers[3]->setPos( 35, 100 );
  chkShowLayers[3]->setCallBack( dummyCall,this );

  //Properties window
  chkPropBoss = new checkBoxClass( chkBoxSpriteId, inp, 0, &sprMan );
  chkPropBoss->setPos( 0, 470 );
  chkPropBoss->setCallBack( chkPropCall, this );
  chkPropWait = new checkBoxClass( chkBoxSpriteId, inp, 0, &sprMan );
  chkPropWait->setPos( 0, 500 );
  chkPropWait->setCallBack( chkPropCall, this );
  chkPropMustLive = new checkBoxClass( chkBoxSpriteId, inp, 0, &sprMan );
  chkPropMustLive->setPos( 0, 530 );
  chkPropMustLive->setCallBack( chkPropCall, this );
  chkPropEnemy = new checkBoxClass( chkBoxSpriteId, inp, 0, &sprMan );
  chkPropEnemy->setPos( 0, 560 );
  chkPropEnemy->setCallBack( chkPropCall, this );


  lblFileName = new labelClass("unnamed",glText,FONT_TINY);
  lblFileName->setBgColor(0.0,0.0,0.1,1);
  lblFileName->setHoverBgColor( 0.1,0.3,0.1,1);
  lblFileName->setSelectedBgColor( 0.3,0.1,0.1,1);
  lblFileName->setTxtColor(1,1,1,1);
  lblFileName->setType(LABEL_EDITABLE,dummyCall,this,inp);
  lblFileName->setPos( glText->getLength("Options for Level: ", FONT_TINY), 451);

  lblPropHiTag=new labelClass("0", glText, FONT_TINY);
  lblPropHiTag->setBgColor(0.0,0.0,0.1,1);
  lblPropHiTag->setHoverBgColor( 0.1,0.3,0.1,1);
  lblPropHiTag->setSelectedBgColor( 0.3,0.1,0.1,1);  lblPropHiTag->setTxtColor(1,1,1,1);
  lblPropHiTag->setPos(300+glText->getLength("HiTag: ",FONT_TINY) ,451+glText->getHeight(FONT_TINY) );
  lblPropHiTag->setType(LABEL_EDITABLE, lblTagEditedCall, this, inp);

  lblPropLoTag=new labelClass("0", glText, FONT_TINY);
  lblPropLoTag->setBgColor(0.0,0.0,0.1,1);
  lblPropLoTag->setHoverBgColor( 0.1,0.3,0.1,1);
  lblPropLoTag->setSelectedBgColor( 0.3,0.1,0.1,1);  lblPropHiTag->setTxtColor(1,1,1,1);
  lblPropLoTag->setTxtColor(1,1,1,1);
  lblPropLoTag->setPos( 300+glText->getLength("HiTag: ",FONT_TINY),451+glText->getHeight(FONT_TINY)*2);
  lblPropLoTag->setType(LABEL_EDITABLE, lblTagEditedCall, this, inp);

  //Load game data
  env = new objLoader(&sprMan); //Holds the info that the editor needs
  spawner = new objSpawner(&sprMan, 1); //Spawns the gameobjects into the gameworld
  map = new mapClass(&sprMan);
}


int editorClass::run()
{

  //Translate mouse location
  mx = (inp->mousex+sceneInfo.camX*pOffset[workLayer])/zoom;
  my = (inp->mousey+sceneInfo.camY*pOffset[workLayer])/zoom;

  //Anything that blocks input?
  if(lblPropHiTag->selected || lblPropLoTag->selected || lblFileName->selected)
  {
    blockInp=1;
  } else {
    blockInp=0;
  }

  //Draw Map
  drawMap();

  if(showcol)
    map->showColZone(); //Debug

  if(showCamGuides)
    drawCamGuides();
  //Draw Interface
  isMapClick=1; //any callback from ui will set to 0. This is used to avoid
                //Placing objects when clicking ui elements
  drawUi();
  if(!isMapClick)
    inp->mouseclick=0;

  //TODO: integrate load/save into ui and use blockInp only.
  //Nasty but load/save textfields were bitches and I didn't want to deal with it
  if(!blockInp)
  {
    //Quit editor?
    if(inp->keyStates[SDLK_ESCAPE])
    {
      resetInput();
      return( 0 );
    }

    //Check if we should scroll - with keyboard
    if(inp->keyStates[SDLK_RIGHT])
      sceneInfo.camX+=2/zoom;
    if(inp->keyStates[SDLK_LEFT])
      sceneInfo.camX-=2/zoom;
    if(inp->keyStates[SDLK_UP])
      sceneInfo.camY-=2/zoom;
    if(inp->keyStates[SDLK_DOWN])
      sceneInfo.camY+=2/zoom;

    //Check if we should scroll - with mouse
    if(inp->mousex < 2)
    {
      sceneInfo.camX-=8/zoom;
    } else if(inp->mousex > sceneInfo.X-3)
    {
      sceneInfo.camX+=8/zoom;
    }
    if(inp->mousey < 2)
    {
      sceneInfo.camY-=8/zoom;
    } else if(inp->mousey > sceneInfo.Y-3)
    {
      sceneInfo.camY+=8/zoom;
    }
    //Check if we should scroll - with z key+mouse position
    if(inp->keyPress==SDLK_z)
    {
      inp->keyPress=0;
      sceneInfo.camX += inp->mousex - (sceneInfo.X/2.0);
      sceneInfo.camY += inp->mousey - (sceneInfo.Y/2.0);
    }

    //toggle layers
    if(inp->keyPress==SDLK_l)
    {
      inp->keyPress=0;
      toggleShownLayers();
    }
    //cam 0,0
    if(inp->keyStates[SDLK_HOME])
    {
      sceneInfo.camX = 0;
      sceneInfo.camY = 0;
    }

    if(sceneInfo.camX < 0)
      sceneInfo.camX=0;
    if(sceneInfo.camY < 0)
      sceneInfo.camY=0;
    //Snap to grid?
    if(inp->keyPress==SDLK_s && !inp->keyStates[SDLK_LCTRL])
    {
      inp->keyPress=0;
      snapToGrid = !snapToGrid;
    }

    //Show camcontrols?
    if(inp->keyPress==SDLK_x)
    {
      inp->keyPress=0;
      showCamGuides = !showCamGuides;
    }

    //Show collision
    if(inp->keyPress==SDLK_c)
    {
      inp->keyPress=0;
      showcol = !showcol;
    }
    //zoom out
    if(inp->mousewheeldown)
    {
      inp->mousewheeldown=0;
      if(zoom > 0.25)
      {
        zoom -= 0.25;
      }
    }
    //zoom in
    if(inp->mousewheelup)
    {
      inp->mousewheelup=0;
      zoom += 0.25;
    }

    //Switch between sprites
    if(inp->keyPress==SDLK_q)
    {
      inp->keyPress=0;
      spawnIndex--;
      if(spawnIndex < env->sectionStart[workType])
        spawnIndex = env->sectionStop[workType];

      toolBaseProp = env->items.at(spawnIndex).props;
    }
    if(inp->keyPress==SDLK_w)
    {
      inp->keyPress=0;
      spawnIndex++;
      if(spawnIndex > env->sectionStop[workType])
        spawnIndex=env->sectionStart[workType];

      toolBaseProp = env->items.at(spawnIndex).props;
    }
    if(inp->keyPress==SDLK_1)
    {
      inp->keyPress=0;
      workType++;
      if(workType==env->numSections) workType=0;
      spawnIndex=env->sectionStart[workType];

      toolBaseProp = env->items.at(spawnIndex).props;
    }

    //Ctrl+n = new
    if(inp->keyStates[SDLK_LCTRL] && inp->keyStates[SDLK_n])
    {
      printc("New level.");
      map->newMap();
      lblFileName->text = "unnamed";
    }

    //Ctrl+s = save
    if(inp->keyStates[SDLK_LCTRL] && inp->keyStates[SDLK_s])
    {
      if(lblFileName->text.compare("unnamed")==0)
      {
        printc("ERROR: Can't save unnamed level file.");
        mapProperties=1;
        itemProperties=0;
        if(!lblFileName->selected)
        {
          lblFileName->selected=1;
          lblFileName->edit();
        }
      } else {
        printc("Saving level '%s'",lblFileName->text.data());
        map->pOffset[1] = pOffset[1];
        map->pOffset[0] = pOffset[0];
        map->saveMap( lblFileName->text );
        //showSave=1;
        if(lblFileName->selected)
          lblFileName->selected=0;
      }
    }

    //Ctrl+l = load
    if(inp->keyStates[SDLK_LCTRL] && inp->keyStates[SDLK_l])
    {
      if(lblFileName->text.compare("unnamed")==0)
      {
        printc("ERROR: Can't load unnamed level file.");
        mapProperties=1;
        itemProperties=0;
        if(!lblFileName->selected)
        {
          lblFileName->selected=1;
          lblFileName->edit();
        }
      } else {
        map->loadMap( lblFileName->text, true );
        pOffset[0] = map->pOffset[0];
        pOffset[1] = map->pOffset[1];

        if(lblFileName->selected)
          lblFileName->selected=0;
      }
    }

    //grid
    if(inp->keyPress==SDLK_g)
    {
      inp->keyPress=0;
      grid=!grid;
    }
    //bounding boxes
    if(inp->keyPress==SDLK_b)
    {
      inp->keyPress=0;
      bounds = !bounds;
    }

    //Help?
    if(inp->keyPress==SDLK_h)
    {
      inp->keyPress=0;
      (help==2)?help=0:help++;
    }

    //Delete grabbed object?
    if(inp->keyStates[SDLK_DELETE] && selectedEntity != -1)
    {

      delete map->objects.at( selectedEntity );
      vector<gameObject*>::iterator delit = map->objects.begin();
      delit += selectedEntity;
      map->objects.erase( delit );
      selectedEntity=-1;
    }

    //Show object properties
    if(inp->keyPress==SDLK_p)
    {
      inp->keyPress=0;
      itemProperties = !itemProperties;
      mapProperties?mapProperties=!mapProperties:mapProperties;
    }
    //Show map options
    if(inp->keyPress==SDLK_o)
    {
      inp->keyPress=0;
      mapProperties = !mapProperties;
      itemProperties?itemProperties=!itemProperties:itemProperties;
    }

    //
    if(inp->keyPress==SDLK_t)
    {
      inp->keyPress=0;
      set.collisionTerrainDetailReduction++;
      map->saveMap( lblFileName->text );
      map->loadMap( lblFileName->text, true );
    }
    if(inp->keyPress==SDLK_y)
    {
      inp->keyPress=0;
      set.collisionTerrainDetailReduction--;
      map->saveMap( lblFileName->text );
      map->loadMap( lblFileName->text, true );
    }

    //Offsets
    if(inp->keyPress==SDLK_7)
    {
      inp->keyPress=0;
      pOffset[0] -= 0.05;
    }
    if(inp->keyPress==SDLK_8)
    {
      inp->keyPress=0;
      pOffset[0] += 0.05;
    }
    if(inp->keyPress==SDLK_9)
    {
      inp->keyPress=0;
      pOffset[1] -= 0.05;
    }
    if(inp->keyPress==SDLK_0)
    {
      inp->keyPress=0;
      pOffset[1] += 0.05;
    }

    //Is any entity selected/grabbed
    if(selectedEntity!=-1)
    {
      toolEditProp = &map->objects.at(selectedEntity)->props;
      GLfloat movex=mx, movey=my;
      if(snapToGrid)
      {
       movex=sx;
       movey=sy;
      }
      map->objects.at(selectedEntity)->position( movex, movey );
    } else {
     toolEditProp = &toolBaseProp;
    }

    //Handle mouseclicks on the map
    if(inp->mouseclick && isMapClick)
    {
      inp->mouseclick=0;
      //Any object attached?
      if(selectedEntity != -1)
      {
        selectedEntity=-1;
      } else {
        //Check to see if any object is clicked on
        selectedEntity = checkClick();
        if(selectedEntity==-1)
        {
          spawn();
        }
      }
    }
    updatePropsChk(); //This must be blocked from input when editing tags.
  } //If not blockInp


  return( 6 );
}

void editorClass::drawGrid(GLfloat fac, GLfloat a)
{
  int x,y;
  x=(int)(sceneInfo.camX*fac)%(int)(GRIDSIZE*zoom);
  y=(int)(sceneInfo.camY*fac)%(int)(GRIDSIZE*zoom);

  glDisable( GL_TEXTURE_2D );
  //Draw a quad to show which is marked (only if snaptogrid is on)
  if(snapToGrid && fac == pOffset[workLayer])
  {
    glPushMatrix();
    glTranslatef( -sceneInfo.camX*pOffset[workLayer], -sceneInfo.camY*pOffset[workLayer], 0 );
    glScalef( zoom, zoom, 0.0 );
    glColor4f( 1, 1,1,0.3);
    glBegin(GL_QUADS);
      glVertex2f( sx, sy );
      glVertex2f( sx+GRIDSIZE, sy );
      glVertex2f( sx+GRIDSIZE, sy+GRIDSIZE );
      glVertex2f( sx, sy+GRIDSIZE );
    glEnd();
    glPopMatrix();
  }


  glColor4f( 0, 1, 0, a );
  for(int xx=-x; xx < sceneInfo.X ; xx+=GRIDSIZE*zoom)
  {
    if(inp->mousex > xx && inp->mousex < xx+GRIDSIZE*zoom && fac == pOffset[workLayer] )
      sx=xx/zoom+(int)(sceneInfo.camX*fac/zoom);

      if(grid)
      {
        sprintf(buf, "%.2f", xx/zoom+(int)(sceneInfo.camX*fac/zoom));
        glEnable(GL_TEXTURE_2D);
        glText->write( buf, FONT_TINY, xx, sceneInfo.Y-glText->getHeight(FONT_TINY) );
        glDisable(GL_TEXTURE_2D);
        glBegin( GL_LINES );
          glVertex2i( xx, 0 );
          glVertex2i( xx, sceneInfo.Y );
        glEnd( );
      }
  }

  for(int yy=-y; yy < sceneInfo.Y ; yy+=GRIDSIZE*zoom)
  {

    if(inp->mousey > yy && inp->mousey < yy+GRIDSIZE*zoom && fac == pOffset[workLayer])
      sy=yy/zoom+(int)(sceneInfo.camY*fac/zoom);

    if(grid)
    {
      sprintf(buf, "%.2f", yy/zoom+(int)(sceneInfo.camY*fac/zoom));
      glEnable(GL_TEXTURE_2D);
      glText->write( buf, FONT_TINY, 00, yy );
      glDisable(GL_TEXTURE_2D);

      glBegin( GL_LINES );
        glVertex2i( 0, yy );
        glVertex2i( sceneInfo.X, yy );
      glEnd( );
    }
  }
  glColor4f( 1, 1, 1, 1 );
  glEnable( GL_TEXTURE_2D );
}

//Shows the camera lines
void editorClass::drawCamGuides()
{
  //Top first
  glPushMatrix();
  glTranslatef( -sceneInfo.camX, -sceneInfo.camY, 0.0);
  glScalef( zoom, zoom, 1.0 );
  glDisable(GL_TEXTURE_2D);
  glColor4f( 0.0, 1.0, 0.0, 1.0);
  for(int i=0; i < 2; i++)
  {
    glBegin(GL_LINE_STRIP);

    for(vector<gameObject*>::iterator it = map->objects.begin(); it != map->objects.end(); ++it)
    {
      //types 2 and 3 are cam limites top and bottom
      if( (*it)->type == 2+i )
      {
        //if it's the first, start at x0
        glVertex2f( (*it)->p.x, (*it)->p.y );
      }
    }
  glEnd();
  }

  glPopMatrix();
}

//"slow" but simple, just interate over all objects for each layer, then ask if it should be drawn
void editorClass::drawMap()
{

  for(int i=0; i < 4; i++)
  {
    if(chkShowLayers[i]->checked)
    {
      //The grid for this layer
      drawGrid( pOffset[i], (1.0/3.0)*(i+1.0) );

      glPushMatrix();
      glTranslatef( -sceneInfo.camX*pOffset[i], -sceneInfo.camY*pOffset[i], 0 );
      glScalef( zoom, zoom, 1.0 );

      //The map
      glColor4f(1,1,1,1);
      glEnable( GL_TEXTURE_2D );
      for(vector<gameObject*>::iterator it = map->objects.begin(); it != map->objects.end(); ++it)
      {
        if((*it)->layer==i)
        {
          (*it)->draw();
          //Show bounding box?
          if(bounds && (*it)->sprite )
          {
            glDisable( GL_TEXTURE_2D );
            glColor3f(1,0,0);
            glBegin( GL_LINE_STRIP );
              glVertex2f( (*it)->p.x,(*it)->p.y );
              glVertex2f( (*it)->p.x+(*it)->sprite->size.x, (*it)->p.y );
              glVertex2f( (*it)->p.x+(*it)->sprite->size.x, (*it)->p.y+(*it)->sprite->size.y );
              glVertex2f( (*it)->p.x, (*it)->p.y+(*it)->sprite->size.y );
              glVertex2f( (*it)->p.x,(*it)->p.y );
            glEnd();
            glColor3f(1,1,1);
            glEnable( GL_TEXTURE_2D );
          }
          //Show hiTag/loTag if !=0
          if((*it)->type > 0)
          {
            if( (*it)->props.hiTag != 0 || (*it)->props.loTag != 0)
            {
              fdot textPos;
              textPos.x = (*it)->p.x;
              textPos.y = (*it)->p.y+(*it)->sprite->size.y;
              sprintf(buf, "%i , %i", (*it)->props.hiTag,(*it)->props.loTag);
              glText->write( buf, FONT_TINY, textPos.x, textPos.y );
            }
          }
        }
      }
      glPopMatrix();
    }
  }
}

int editorClass::checkClick()
{
  int pos=0;
  int found=-1;
  for(vector<gameObject*>::iterator it = map->objects.begin(); it != map->objects.end(); ++it)
  {
    if((*it)->layer==workLayer)
    {
      if( mx > (*it)->p.x && mx < (*it)->p.x+(*it)->sprite->size.x )
      {
        if( my > (*it)->p.y && my < (*it)->p.y+(*it)->sprite->size.y )
        {
          found=pos;
        }
      }
    }
    pos ++;
  }

  return(found);

}

void editorClass::drawUi()
{
  glEnable( GL_TEXTURE_2D );
  glColor4f( 1,1,1,1 );
  for(int i=0; i < 4; i++)
  {
    chkWorkLayer[i]->draw();
    chkShowLayers[i]->draw();
  }

  //Messages:
  buf[0] = 0;

  sprintf(buf, "Cam[%i,%i] Cur[%i,%i] Zoom[%.2f]",(int)sceneInfo.camX, (int)sceneInfo.camY, (int)mx, (int)my, (float)zoom);

  if(snapToGrid)
    sprintf(buf, "%s Snap[%.2f,%.2f]",buf, (float)sx,(float)sy);

  //Show what's selected
  if(selectedEntity != -1)
  {
    sprintf(buf, "%s Selected [obj:%i Type:%i Layer:%i Pos: %.0f,%.0f]",
    buf, selectedEntity, map->objects.at(selectedEntity)->type,
    map->objects.at(selectedEntity)->layer,map->objects.at(selectedEntity)->p.x,map->objects.at(selectedEntity)->p.y );
  }

  glText->writeoutlined(buf, FONT_TINY, 0, sceneInfo.Y-glText->getHeight(FONT_TINY)*2);

  sprintf(buf, "Browsing: %s", txtWorkTypeNames[workType/2].data());
  glText->writeoutlined(buf, FONT_TINY, 0, sceneInfo.Y-glText->getHeight(FONT_TINY));

  //Show selected sprite
  env->items.at(spawnIndex).sprite->position( sceneInfo.X - env->items.at(spawnIndex).sprite->size.x-10, 10 );
  env->items.at(spawnIndex).sprite->draw();
  //name of sprite
  glText->writeoutlined( env->items.at(spawnIndex).strName, FONT_TINY, sceneInfo.X-glText->getLength( env->items.at(spawnIndex).strName, FONT_TINY )-10, 10+env->items.at(spawnIndex).sprite->size.y );

  //Item Properties?
  if(itemProperties || mapProperties)
  {
    //Disable mouseclicks inside the rect
    if(inp->mousey > 449 )
    {
      isMapClick=0;
    }
    //
    glDisable( GL_TEXTURE_2D );
    glColor4f( 0.1, 0.1, 0.1, 0.9 );
    glBegin( GL_QUADS );
      glVertex2i( 0, 450 );
      glVertex2i( sceneInfo.X, 450 );
      glVertex2i( sceneInfo.X, sceneInfo.Y );
      glVertex2i( 0, sceneInfo.Y );
    glEnd();
    glColor4f( 1, 1, 1, 1 );
    glBegin( GL_LINES );
      glVertex2i( 0, 450 );
      glVertex2i( sceneInfo.X, 450 );
    glEnd();
    glEnable( GL_TEXTURE_2D );

    if(itemProperties)
    {
      if(selectedEntity != -1)
        sprintf(buf, "Properties for object %i.", selectedEntity);
      else
        sprintf(buf, "Properties for new objects.");
      glText->writeoutlined(buf, FONT_TINY, 1, 451);

      chkPropBoss->draw();
      glText->writeoutlined( "Win if die", FONT_TINY, chkPropBoss->px+chkPropBoss->sprite[0]->size.x,chkPropBoss->py+chkPropBoss->sprite[0]->halfSize.y-(glText->getHeight(FONT_TINY)/2) );
      chkPropWait->draw();
      glText->writeoutlined( "WaitToImDead", FONT_TINY, chkPropWait->px+chkPropWait->sprite[0]->size.x,chkPropWait->py+chkPropWait->sprite[0]->halfSize.y-(glText->getHeight(FONT_TINY)/2) );
      chkPropMustLive->draw();
      glText->writeoutlined( "Lose if die", FONT_TINY, chkPropMustLive->px+chkPropMustLive->sprite[0]->size.x,chkPropMustLive->py+chkPropMustLive->sprite[0]->halfSize.y-(glText->getHeight(FONT_TINY)/2) );
      chkPropEnemy->draw();
      glText->writeoutlined( "Enemy", FONT_TINY, chkPropEnemy->px+chkPropEnemy->sprite[0]->size.x,chkPropEnemy->py+chkPropEnemy->sprite[0]->halfSize.y-(glText->getHeight(FONT_TINY)/2) );

      glText->writeoutlined( "HiTag:", FONT_TINY, 300, 451+glText->getHeight(FONT_TINY) );
      lblPropHiTag->draw();
      glText->writeoutlined( "LoTag:", FONT_TINY, 300, 451+glText->getHeight(FONT_TINY)*2);
      lblPropLoTag->draw();

    } //Item properties
    if(mapProperties)
    {
      sprintf(buf, "Options for Level: \n" );
      sprintf(buf, "%s  TerrainReduction: %i\n  CollisionLines: %i\n",buf, set.collisionTerrainDetailReduction, map->lines.size() );
      sprintf(buf, "%s  Objects %i\n", buf, map->objects.size() );
      sprintf(buf, "%s  Parallax Offsets: %.2f & %.2f\n",buf, pOffset[0], pOffset[1]);
      glText->writeoutlined(buf, FONT_TINY, 1, 451);
      lblFileName->draw();

    }
  } //Properties

  //Show help?
  glText->writeoutlined(txthelp[help], FONT_TINY, sceneInfo.X/2.0-glText->getLength(txthelp[help], FONT_TINY)/2.0, 1);
}

void editorClass::spawn()
{
  GLfloat spawnx=mx, spawny=my;
  if( snapToGrid )
  {
    spawnx=sx;
    spawny=sy;
  }
  string tileFileName = spawner->loader->items.at(spawnIndex).sprite->getFileName();
  int objectType = spawner->loader->items.at(spawnIndex).type;
  map->objects.push_back(spawner->create( objectType, tileFileName, spawnx, spawny, workLayer, &toolBaseProp ));
}

void editorClass::toggleShownLayers()
{
  static int state=0; //show first
  state++;
  if(state > 4)
    state=0;

  if(state==4)
  {
    for(int i=0; i < 4; i++)
    {
      chkShowLayers[i]->checked=1;
    }
  } else {
    for(int i=0; i < 4; i++)
    {
      if(i==state)
      {
        workLayer=i;
        chkShowLayers[i]->checked=1;
        chkWorkLayer[i]->checked=1;
        chkWorkLayerLast[i]=1;
      } else {
        chkShowLayers[i]->checked=0;
        chkWorkLayer[i]->checked=0;
        chkWorkLayerLast[i]=0;
      }
    }
  }
}

//Updates the props checkbox buttons to the pointer
void editorClass::updatePropsChk()
{
  chkPropWait->checked=toolEditProp->wait;
  chkPropBoss->checked=toolEditProp->boss;
  chkPropMustLive->checked=toolEditProp->mustLive;
  chkPropEnemy->checked=toolEditProp->enemy;

  sprintf(buf, "%i", toolEditProp->hiTag);
  lblPropHiTag->text=buf;
  sprintf(buf, "%i", toolEditProp->loTag);
  lblPropLoTag->text=buf;
}


///reset input
void editorClass::resetInput()
{
  toolBaseProp.hiTag=0;
  toolBaseProp.loTag=0;

  toolEditProp=&toolBaseProp;

  snapToGrid=0;
  bounds=1;
  selectedEntity=-1;
  itemProperties=0;
  mapProperties=0;
  grid=1;
  showcol=1;
  showCamGuides=1;
  zoom=1.0;
  sceneInfo.camX=0;
  sceneInfo.camY=0;
  workLayer=2;
  workType=0;
  spawnIndex=0;
  inp->mousex=400;
  inp->mousey=300;
  inp->mouseclick=0;
  inp->mousedown=0;
  sceneInfo.camX=0;
  sceneInfo.camY=0;

}

//Callbacks
void editorClass::dummyCall( void* c )
{
  static_cast<editorClass*>(c)->isMapClick=0;
}

void editorClass::chkWorkLayerCall( void* c )
{
  editorClass* that = static_cast<editorClass*>(c);
  that->isMapClick=0;
  //Loop through
  for(int i=0; i < 4; i++)
  {
    if(that->chkWorkLayerLast[i])
    {
      that->chkWorkLayer[i]->checked=0;
      that->chkWorkLayerLast[i]=0;
    } else {
      if(that->chkWorkLayer[i]->checked)
      {
        that->workLayer = i;
        that->chkWorkLayerLast[i]=1;
      }
    }
  }
}

void editorClass::chkPropCall( void* c)
{
  editorClass* that = static_cast<editorClass*>(c);

  that->toolEditProp->boss=that->chkPropBoss->checked;
  that->toolEditProp->wait=that->chkPropWait->checked;
  that->toolEditProp->mustLive=that->chkPropMustLive->checked;
  that->toolEditProp->enemy=that->chkPropEnemy->checked;
}

void editorClass::lblTagEditedCall( void* c )
{
  editorClass* that = static_cast<editorClass*>(c);
  that->toolEditProp->hiTag= atoi( that->lblPropHiTag->text.data() );
  that->toolEditProp->loTag= atoi( that->lblPropLoTag->text.data() );
}
