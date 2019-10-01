#include "level.hpp"
extern struct sceneInfo_t sceneInfo;
extern settingsClass set;

mapClass::mapClass(spriteManagerClass* sp)
{
  sprMan=sp;
  col=NULL;
}

void mapClass::newMap()
{
 for(vector<gameObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
  {
    delete (*it);
  }
  objects.clear();
  if(col)
  {
    //remove the map
    for(int i=0; i < w; i++)
    {
      delete[] col[i];
      delete[] colb[i];
    }

    delete[] col;
    delete[] colb;
    col=NULL;
    colb=NULL;
  }
  colLines.clear();

}

mapClass::~mapClass()
{
  newMap();
}

static bool compFunc(gameObject* a, gameObject* b)
{
  return( a->p.x < b->p.x );
}

bool mapClass::loadMap(string fileName, bool inEditor)
{
  //empty whatever might allready be there
  newMap();

  mapFileHeader header;
  header.version=-1;

  vector<mapFileTile> tiles;
  tiles.clear();
  mapFileTile ttil;

  vector<mapFileObj> objs;
  objs.clear();
  mapFileObj tobj;

  fstream file(fileName.data(), ios::binary|ios::in);

  if(!file.is_open())
  {
    printc("Error: Couldn't open '%s' for reading.",fileName.data());
    return(0);
  }

  file.read( reinterpret_cast<char*>(&header), sizeof(mapFileHeader) );

  printc("Level file: '%s'.",fileName.data());
  printc("  Header version: %i",(int)header.version);
  printc("  Tiles: %i",header.numTiles);
  printc("  Objects: %i",header.numObjects);

  //check if this level is the right version.
  if( (int)header.version != LEVELVERSION)
  {
    printc("Error: Wrong levelfile version.");
    return(0);
  }

  set.collisionTerrainDetailReduction=header.terrainReduction;
  pOffset[0]=header.pOffset[0];
  pOffset[1]=header.pOffset[1];

  //read tiles
  for(int i=0; i < header.numTiles; i++)
  {
    file.read( reinterpret_cast<char*>(&ttil), sizeof(mapFileTile) );
    tiles.push_back(ttil);
  }

  //Read objects
  for(int i=0; i < header.numObjects; i++)
  {
    file.read( reinterpret_cast<char*>(&tobj), sizeof(mapFileObj) );
    objs.push_back(tobj);
  }

  printc("  Read %i tiles and %i objects.",tiles.size(),objs.size() );
  file.close();

  objSpawner* spawner = new objSpawner(sprMan, inEditor);
  spawner->bMan=bMan;

  //For figuring out map size
  line mapSize;
  mapSize.a.x=0;
  mapSize.b.x=-1;
  mapSize.a.y=0;
  mapSize.b.y=-1;

  gameObject* tempObject; //Used to figure out lvl size
  //Spawn tiles and objects as gameObjects.
  goProperties tempProps; //just temp
  for(vector<mapFileTile>::iterator it=tiles.begin(); it!=tiles.end(); ++it)
  {
    tempObject = spawner->create( objTile,it->file, (GLfloat)it->x, (GLfloat)it->y, it->layer, &tempProps );
    objects.push_back(tempObject);

    //Figure out level size

    if(it->x < mapSize.a.x)
    {
      mapSize.a.x=it->x;
    }
    if(it->x+tempObject->sprite->size.x+4 > mapSize.b.x)
    {
      mapSize.b.x=it->x+tempObject->sprite->size.x+2;
    }
    if(it->y < mapSize.a.y)
    {
      mapSize.a.y=it->y;
    }
    if(it->y+tempObject->sprite->size.y+4 > mapSize.b.y)
    {
      mapSize.b.y=it->y+tempObject->sprite->size.y+2;
    }
  }

  //Spawn objects
  for(vector<mapFileObj>::iterator it=objs.begin(); it!=objs.end(); ++it)
  {
    objects.push_back(spawner->create( it->type,"", (GLfloat)it->x, (GLfloat)it->y, it->layer, &it->prop ));
  }

  w=(int)(mapSize.a.x*-1)+(int)mapSize.b.x;
  h=(int)(mapSize.a.y*-1)+(int)mapSize.b.y;
  ho=(int)mapSize.a.y;
  wo=(int)mapSize.a.x;

  if(w < 1) w=1;
  if(h < 1) h=1;


  //Sort so smallest x pos is first
  sortMap();

  //debug
 /* for(vector<gameObject*>::iterator it=objects.begin(); it!=objects.end();++it)
  {
    cout << "Object at " << (*it)->px << endl;
  }*/

  delete spawner;
  makeCollisionZones();
  return(1);
}


//save
void mapClass::saveMap(string fileName)
{
mapFileHeader header;


  vector<mapFileTile> tiles;
  tiles.clear();
  mapFileTile ttil;

  vector<mapFileObj> objs;
  objs.clear();
  mapFileObj tobj;


  //Fill the two new vectors
  for(vector<gameObject*>::iterator it=objects.begin(); it != objects.end(); ++it)
  {
    if( (*it)->type==0 )
    {
      sprintf(ttil.file, "%s", (*it)->sprite->getFileName().data());
      ttil.layer=(*it)->layer;
      ttil.x=(*it)->p.x;
      ttil.y=(*it)->p.y;
      tiles.push_back(ttil);
    } else {
      tobj.layer=(*it)->layer;
      tobj.prop=(*it)->props;
      tobj.type=(*it)->type;
      tobj.x=(*it)->p.x;
      tobj.y=(*it)->p.y;
      objs.push_back( tobj );
    }
  }

  header.version=LEVELVERSION;
  header.numObjects=objs.size();
  header.numTiles=tiles.size();
  header.pOffset[0]=pOffset[0]; //Farthest background
  header.pOffset[1]=pOffset[1]; //Background 1

  //Override setting if not overwritten.
  if(!set.overrideDefaultDetailReduction)
    header.terrainReduction=set.collisionTerrainDetailReduction;

  fstream file(fileName.data(), ios::binary|ios::out|ios::trunc);

  //Write header
  file.write(reinterpret_cast<char*>(&header), sizeof(mapFileHeader));

  //Write tiles
  for(vector<mapFileTile>::iterator it=tiles.begin(); it!=tiles.end();++it)
  {
    file.write(reinterpret_cast<char*>(&(*it)),sizeof(mapFileTile));
  }

  //Write objects
  for(vector<mapFileObj>::iterator it=objs.begin(); it!=objs.end();++it)
  {
    file.write(reinterpret_cast<char*>(&(*it)),sizeof(mapFileObj));
  }

  file.close();
}

void mapClass::makeCollisionZones()
{

  //Allocate bool area(s) for collision points
  col = new bool*[w]; //This is filled with data from the texture
  colb = new bool*[w];  //This is filled with optimised data from col
  for(int i=0; i < w; i++)
  {
    col[i] = new bool[h];
    colb[i] = new bool[h];
    for(int ii=0; ii < h; ii++)
    {
      col[i][ii]=0;
      colb[i][ii]=0;
    }
  }

  GLubyte* imgdata;
  int px,py,s;

  for(vector<gameObject*>::iterator it=objects.begin(); it!=objects.end(); ++it)
  {
    if((*it)->layer==2)
    {
      if((*it)->type==0)
      {
        if((*it)->sprite)
        {
          if((*it)->sprite->playing==0)
          {
            //Allocate pixels for texture
            px=(int)(*it)->sprite->size.x+2;
            py=(int)(*it)->sprite->size.y+2;
            s = px*py*4;
            //cout << "Image " << (*it)->sprite->getFileName() << " is " << px <<"x"<<py<<" px"<<endl;
            imgdata=new GLubyte[s];
            glBindTexture( GL_TEXTURE_2D, (*it)->sprite->tex() );
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgdata);

            int x=0,y=0;
            for(int i=0; i < s; i+=4)
            {

              if((int)imgdata[i+3] > 128)
              {
                if( x > 0 && x < px-1 && y>0 && y < py-1 )
                {

             //     cout << "(py:" << (*it)->py <<" .y:" <<y<<" - type: " << (*it)->type <<endl;
             //     cout << "Trying px " << x << "x"<< y<< "(" << (int)(*it)->px+x <<"x"<< (int)(*it)->py+y <<")"<<endl;
                  col[ (int)(*it)->p.x+x-wo-1 ][ (int)(*it)->p.y+y-ho ] = 1;
                }
              }

              x++;
              if(x > px-1)
              {
                x=0;
                y++;
              }

            }

            delete [] imgdata;
          }
        }
      }
    }
  }

  //Hollow out the pixels (only copy pixels that don't have neighbors all around)
  for(int x=0; x < w; x++)
  {
    for(int y=0; y < h; y++)
    {
      if(x > 0 && x < w-1 && y > 0 && y < h-1)
      {
        if(col[x][y])
        {
          if(col[x-1][y] && col[x+1][y] && col[x][y-1] && col[x][y+1])
          {
            colb[x][y]=0;
          } else {
            colb[x][y]=1;
          }
        }
      }
    }
  }

  //This is an attempt to break down the pixels into a set of line strips.
  for(int x=0; x < w; x++)
  {
    for(int y=0; y < h; y++)
    {
      if(colb[x][y])
      {
        follow(x,y);
      }
    }
  }

  //Now convert those line strips to lines.
  line tempLine;
  fdot prevDot;
  GLfloat endX,endY,len,tan;

  lines.clear();
  int lineCount=0;
  for(vector< vector<fdot> >::iterator it=colLines.begin(); it!= colLines.end(); ++it)
  {
    bool first=1;
    for(vector<fdot>::iterator itt=it->begin(); itt!=it->end(); ++itt)
    {
      if(first)
      {
        first=0;
        prevDot=*itt;
      } else {
        tempLine.a.x=prevDot.x+wo;
        tempLine.a.y=prevDot.y+ho;
        tempLine.b.x=itt->x+wo;
        tempLine.b.y=itt->y+ho;

        //Endpoints relative to 0,0 (eefect of abs?????????????????)
        endX=abs(tempLine.b.x)-abs(tempLine.a.x);
        endY=abs(tempLine.b.y)-abs(tempLine.a.y);

        //Vector Length
        len=sqrt( pow( endX, 2) + pow( endY, 2) );


        tan=atan2f(endY,endX);

        tempLine.normal.x = -cos( tan+1.57079633 );
        tempLine.normal.y = -sin( tan+1.57079633 );

        tempLine.middle.x = (tempLine.b.x - tempLine.a.x)/2.0;
        tempLine.middle.y = (tempLine.b.y - tempLine.a.y)/2.0;


        lines.push_back(tempLine);
        prevDot=*itt;
      }
    }
  }

}

void mapClass::follow(int x, int y)
{
  bool stop=0;
  plotLines.clear();

  plot(x,y,-2,0); //Tells plot to clean up

  while(!stop)
  {
    if(x < w+1 && colb[x+1][y])
    {
      plot(x,y, 1,0 );
      x++;
    } else if(y > 0 && colb[x][y-1])
    {
      plot(x,y, 0,-1 );
      y--;
    } else if( y < h-1 && colb[x][y+1])
    {
      plot(x,y, 0,1 );
      y++;
    } else if(x > 0 && colb[x-1][y])
    {
      plot(x,y, -1,0 );
      x--;
    } else if(x > 0 && y > 0 && colb[x-1][y-1])
    {
      plot(x,y, -1,-1 );
      x--;
      y--;
    } else if(x <w-1 && y > 0 && colb[x+1][y-1])
    {
      plot(x,y, 1,-1 );
      x++;
      y--;
    } else if(x < w-1 && y < h-1 && colb[x+1][y+1])
    {
      plot(x,y, 1,1 );
      x++;
      y++;
    } else if( x > 0 && y < h-1 && colb[x-1][y+1])
    {
      plot(x,y, -1,+1 );
      x--;
      y++;
    } else {
      stop=1;
    }

  }

  plotLines.push_back( plotLines.at(0) );

  //Optimize
  fdot prev = plotLines.at(0);
  int reduce=set.collisionTerrainDetailReduction;

  bool erased=1;
  while(erased)
  {
    erased=0;
    for(vector<fdot>::iterator it= plotLines.begin(); it!=plotLines.end();++it)
    {
      //If a line is not diffrent than the previous, remove it

      if( (it->x < prev.x+reduce && it->x > prev.x-reduce) && (it->y < prev.y+reduce && it->y > prev.y-reduce) )
      {
        if(it!=plotLines.end() && it!= plotLines.begin())
        {
          it=plotLines.erase(it);
          erased=1;
        }
      }
      if(it==plotLines.end())
      {
        break;
      }
      prev=*it;
    }
  }

  //Only use strips with more than 2 lines.
  if(plotLines.size() > 2)
  {
    colLines.push_back( plotLines );
  } else {
   printc("Optimized.");
  }
}

void mapClass::plot(int x, int y, int dx,int dy)
{
  static int oldx=0, oldy=0;
  static fdot tl; //temp line
  static fdot oldpos;

  //nasty hack
  if(dx==-2)
  {
   oldx=0;
   oldy=0;
   oldpos.x=x;
   oldpos.y=y;
   return;
  }

  if(oldx==dx && oldy==dy)
  {
    colb[x+dx][y+dy]=0;
    oldpos.x=x+dx;
    oldpos.y=y+dy;
  } else {
    if(oldpos.x !=-24 && oldpos.y !=-42) //Okay, this is wrong, but i don't think anything will ever end up there so well..
    {
      plotLines.push_back(oldpos);
      oldpos.x=-24;
      oldpos.y=-42;
    }

    tl.x=x+dx;
    tl.y=y+dy;
    plotLines.push_back(tl);
    colb[x+dx][y+dy]=0;
  }
  oldx=dx;
  oldy=dy;
}


void mapClass::showColZone()
{
  if(!col)
    return;

    glPushMatrix();
    glTranslatef( -sceneInfo.camX, -sceneInfo.camY, 0.0);
//    glScalef( zoom, zoom, 1.0 );

    glDisable( GL_TEXTURE_2D );
    glBegin( GL_LINES );
    for(vector<line>::iterator it=lines.begin(); it!=lines.end(); ++it)
    {
      glColor4f( 0,0,1,1.0 );
      glVertex2f( it->a.x, it->a.y );
      glColor4f( 1,1,1,1.0 );
      glVertex2f( it->b.x, it->b.y );

      glColor4f( 1,1,0,1.0 );
      glVertex2f( it->a.x+it->middle.x, it->a.y+it->middle.y);
      glColor4f( 1,0,0,1.0 );
      glVertex2f( it->a.x+it->middle.x+it->normal.x*10, it->a.y+it->middle.y+it->normal.y*10);

    }
    glEnd();

    glPopMatrix();
}

void mapClass::sortMap()
{
  sort( objects.begin(), objects.end(), compFunc );
}
