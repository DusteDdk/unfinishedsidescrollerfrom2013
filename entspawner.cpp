#include "entspawner.hpp"

extern dirsClass dir;

int objLoader::strToObjId(string s)
{

  if( s == "playerShip" ) return( objPlayerShip );
  if( s == "camLimBottom" ) return( objCamLimitBottom );
  if( s == "camLimTop" ) return( objCamLimitTop );
  if( s == "effector" ) return( objFx );
  if( s == "error" ) return( objErr );

  if( s == "cargoBox" ) return( objCargoBox );
  if( s == "repairKit" ) return( objRepairKit );
  if( s == "battery" ) return( objShieldPower );

  if( s == "astroid0" ) return( objAstroid0 );
  if( s == "astroid1" ) return( objAstroid1 );
  if( s == "astroid2" ) return( objAstroid2 );
  if( s == "astroid3" ) return( objAstroid3 );

  if( s == "cutter1" ) return( objCutter1 );


  //Error
  return( objErr );
}

int objLoader::objTypeToIndex(int type)
{
  for(int i=0; i < items.size(); i++)
  {
//    printc("Type check; %i", items.at(i).type);
    if(items.at(i).type == type) return(i);
  }
  //To stop it recursing if it couldn't find the error obj
  if( type == objErr)
  {
    printc("Error, couldn't find the error object, returning first object.");
    return(0);
  } else {
    printc("loader don't have obj type %i", type);
    return( objTypeToIndex(objErr) );
  }
}

//Load list of non-tile objects from file
objLoader::objLoader(spriteManagerClass* sprMan)
{
  numObjs=0;

  //First we load all the tiles directly from the directory
  int numTiles=0;
  struct dirent *dent; //Directory Entry
  DIR *pdir;
  string temp = dir.data+"gfx/env/";

  pdir = opendir( temp.data() );

  pClear();

  //typeOffsets[tileStart]=numObjs;
  sectionStart.push_back( 0 );
  if (pdir)
  {
    while ((dent=readdir(pdir)))
    {
      //We're not going to read hidden files
      if(dent->d_name[0] != '.')
      {
        temp.clear();
        temp.append("gfx/env/");
        temp.append(dent->d_name);
        tempObject.type=objTile;
        tempObject.strName = dent->d_name;
        tempObject.sprite=sprMan->spawn( sprMan->load( temp ) );
        items.push_back( tempObject );
        numObjs++;
        numTiles++;
      }
    }
  }
  sectionStop.push_back( numObjs-1 );
  // End tile loading part

  numSections=1;
  sectionStart.push_back( numObjs );

//NEW CODE
  const int pNum = 10; //number of properties that is read from file.

  string fn = dir.data + "objects.txt";
  ifstream file;
  string line;
  file.open(fn.data(), ios::in);
  if(!file.is_open())
  {
    printc("Fatal: Couldn't open '%s' for reading.", fn.data());
    return;
  }

  int lineNum = 0;
  int foundVars = 0;
  pClear();
  while(!file.eof())
  {
    getline(file, line);
    lineNum++;

    //Strip comments
    int commentPos = line.find("//");
    if(commentPos != string::npos)
    {
      line = line.substr(0, commentPos);
    }

    //Ignore lines shorter than 3 chars.
    if(line.size()>2)
    {


        if(line=="[[section]]")
        {
          pPush();
          pClear();
          foundVars=0;
          //as for now, we just keep compatibility with whatevers working
          //this means that only 1 section is supported
          sectionStop.push_back( numObjs-1 );
          sectionStart.push_back( numObjs );
          numSections++;
        } else {
          int lastChar = line.size()-1;
          if(line[0] == '[' && line[lastChar]==']')
          {
            if(foundVars != 0)
            {
              pPush();
              pClear();
              foundVars=0;
            }
            foundVars++;
            gotName=true;
            tempObject.strName=line.substr(1, lastChar-1);
          }
        }

        //Find val:var
        int delimPos=line.find(":");
        if(delimPos != string::npos)
        {
          string var = line.substr(0, delimPos);
          string val = line.substr(delimPos+1, line.size()-delimPos);

          foundVars++; //We will subtract from if it was not valid
          //Match up with the vars to set:
          if( var == "sprite" )
          {
            tempObject.sprite = sprMan->spawnByFile( val );
          } else if( var == "health" )
          {
            tempObject.health = atoi( val.data() );
          } else if( var == "colDmg" )
          {
            tempObject.colDmg = atoi( val.data() );
          } else if( var == "accelSpeed" )
          {
            tempObject.accelSpeed = splitCoord( val );
          } else if( var == "maxSpeed" )
          {
            tempObject.maxSpeed = splitCoord( val );
          } else if( var == "cruiseSpeed" )
          {
            tempObject.cruiseSpeed = splitCoord( val );
          } else if( var == "movePattern" )
          {
            tempObject.movePattern = atoi( val.data() );
          } else if( var == "shootPattern" )
          {
            tempObject.shootPattern = atoi( val.data() );
          } else if( var == "introPattern" )
          {
            tempObject.introPattern = atoi( val.data() );
          } else {
            printc("'%s':%i > '%s' orphan or unknown var.", fn.data(), lineNum, line.data() );
            foundVars--;
          }
        } //Found delim

    } //At least 3 chars long

    //Did we find all vars ?
    if(foundVars == pNum)
    {
      pPush();
      pClear();
      foundVars=0;
    }

  } //Looping through file

  //Push back the last if it was not full defined)
  {
    foundVars=0;
    pPush();
    pClear();
  }

  sectionStop.push_back( numObjs-1 );
  numSections++;
  printc("objLoader: %i tiles and %i objects loaded.", numTiles, numObjs-numTiles);
}

void objLoader::pClear()
{
  gotName=false;

  //Properties (why not load those from file too?)
  tempObject.props.boss=0;
  tempObject.props.mustLive=0;
  tempObject.props.spawnbehind=0;
  tempObject.props.enemy=1; //Pr default new objects are enemies
  tempObject.props.wait=0;
  tempObject.props.hiTag=0;
  tempObject.props.loTag=0;

  tempObject.sprite = NULL;
  tempObject.health = 0;
  tempObject.colDmg = 0;
  tempObject.accelSpeed = splitCoord( "0,0" );
  tempObject.maxSpeed = splitCoord( "0,0" );
  tempObject.cruiseSpeed = splitCoord( "0,0" );
  tempObject.movePattern =  0;
  tempObject.shootPattern = 0;
  tempObject.introPattern = 0;
}

void objLoader::pPush()
{
  if( tempObject.strName != "error" && strToObjId(tempObject.strName) == objErr )
  {
    printc("objLoader: Object name '%s' defined but not recognized.", tempObject.strName.data() );
  }
  //Resolve name to a objID:
  tempObject.type = strToObjId( tempObject.strName );
  //Push back gameObject
  items.push_back( tempObject );
  numObjs++;
}

fdot objLoader::splitCoord(string s)
{
  fdot c;

  int p = s.find(",");
  if( p == string::npos || s.size() < 3 )
  {
    printc("SplitCoord: Invalid coordinate: '%s'", s.data() );
    c.x=0;
    c.y=0;
  } else {
    c.x = atof( s.substr( 0, p ).data() );
    c.y = atof( s.substr( p, s.size() -p ).data() );
  }
  return(c);
}

objSpawner::objSpawner( spriteManagerClass* s, bool inEdit )
{
  bMan=NULL; //For when in editor (does not matter anyway)
  isInEditor=inEdit;
  sprMan=s;
  loader=new objLoader(sprMan);
}

objSpawner::~objSpawner()
{
  delete loader;
}



//Copy gameObject data to
gameObject* objSpawner::create(int objType, string fileName, GLfloat x, GLfloat y, int layer, goProperties* prop)
{
  gameObject* temp;

  //Find object Index in loader->items
  int objIndex = loader->objTypeToIndex(objType);

  //If it's not a tile, spawn
  spriteClass* spr=NULL;
  if(objType != objTile)
  {
    spr = sprMan->spawnByFile(loader->items.at(objIndex).sprite->getFileName());
  }

  switch(objType)
  {
    case objTile:
      temp = new gameObject;
      temp->sprite = sprMan->spawnByFile( fileName );
      temp->layer=layer;
      temp->sprite->position(x,y);
    break;
    case objPlayerShip: //Player starter position
      temp = new playerShip;
      temp->layer=layer;
    break;
    case objCamLimitTop: //Camera limit top
      temp = new camLimit( false );
      temp->layer=3;
    break;
    case objCamLimitBottom: //Camera limit bototom
      temp = new camLimit( true );
      temp->layer=3;
    break;
    case objFx:
      if(isInEditor)
        temp = new effectorClass( x,y, sprMan );
      else
        temp = new effectorClass();
      temp->layer=3;
    break;
    case objErr:
      temp = new gameObject;
      temp->layer=layer;
    break;
    case objAstroid0:
    case objAstroid1:
    case objAstroid2:
    case objAstroid3:
      temp = new astroid( spr );
      temp->layer=layer;
    break;
    //CargoBox in space
    case objCargoBox:
      temp = new cargoBoxInSpace( spr );
      temp->layer=layer;
    break;
    //TODO:: MAke the objs...
    case objNukeFlying: //The nuke projectile? TODO::Is this what we do with projectiles at all?
      temp = new gameObject;
      temp->layer=layer;
    break;
    case objShieldPower:
      temp = new gameObject;
      temp->layer=layer;
    break;
    case objRepairKit:
      temp = new gameObject;
      temp->layer=layer;
    break;
    case objCutter1:
      temp = new gameObject;
      temp->layer=layer;
    break;
  }



  //if it's not a tile, and don't have a sprite (assigned by ctor)
  temp->props = *prop; //put props in place
  if(objType != objTile)
  {
    temp->bMan=bMan;
    if(!temp->sprite)
    {
      temp->sprite= NULL;
      //Consider special objects in game-mode
      if( (!isInEditor && objType != objCamLimitBottom && objType != objCamLimitTop && objType != objFx) || isInEditor )
      {
        temp->sprite = sprMan->spawnByFile( loader->items.at(objIndex).sprite->getFileName() );
      }
    }
  }

  //Transfer the rest of the vars
  temp->type = objType;

  temp->strName = loader->items.at(objIndex).strName;
  temp->health = loader->items.at(objIndex).health;
  temp->colDmg = loader->items.at(objIndex).colDmg;
  temp->accelSpeed = loader->items.at(objIndex).accelSpeed;
  temp->maxSpeed = loader->items.at(objIndex).maxSpeed;
  temp->cruiseSpeed = loader->items.at(objIndex).cruiseSpeed;
  temp->movePattern = loader->items.at(objIndex).movePattern;
  temp->shootPattern = loader->items.at(objIndex).shootPattern;
  temp->introPattern = loader->items.at(objIndex).introPattern;

  //Position
  temp->p.x = x;
  temp->p.y = y;

  //To update "lastpos" to the current, but only if has sprite
  if(temp->sprite)
  {
    temp->position(x,y);
    temp->position(x,y);
  }
  return(temp);
}
