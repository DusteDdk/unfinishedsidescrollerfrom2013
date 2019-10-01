#include "settings.hpp"

extern dirsClass dir;
void settingsClass::load()
{
    file = dir.save+"/settings.cfg";
    ifstream f; //File
    string line,set,val;
    f.open( file.data() );

    collisionTerrainDetailReduction=6;
    overrideDefaultDetailReduction=0;
    gameX = 800;
    gameY = 600;
    fullScreen = 0;
    lastCareer = -1;
    nagScreen = 1;
    noFbo=0;

    //Read fileinfo
    if(f.is_open())
    {
      while(!f.eof())
      {
        getline(f, line);
        if(line.find('=') != string::npos)
        {
          set=line.substr(0,line.find('='));
          val=line.substr(line.find('=')+1);
          if(set=="resx")
          {
            gameX=atoi(val.data());
          }
          if(set=="resy")
          {
            gameY=atoi(val.data());
          }
          if(set=="fullscreen")
          {
            fullScreen=atoi(val.data());
          }
          if(set=="lastcareer")
          {
            lastCareer=atoi(val.data());
          }
          if(set=="terrainCollisionDetailReduction")
          {
            collisionTerrainDetailReduction=atoi(val.data());
          }
          if(set=="overrideDefaultDetailReduction")
          {
            overrideDefaultDetailReduction=atoi(val.data());
          }
          if(set=="showFps")
          {
            showFps=atoi(val.data());
          }
          if(set=="nagScreen")
          {
            nagScreen=atoi(val.data());
          }
        }
      }
      f.close();
    } else {
      printc("Couldn't load '%s' using defaults.", file.data());
    }
}

void settingsClass::save()
{
  ofstream save;
  save.open( file.data() , ios::out | ios::trunc );
  if(save.is_open())
  {
    save << "resx=" << gameX << endl;
    save << "resy=" << gameY << endl;
    save << "fullscreen=" << fullScreen << endl;
    save << "lastcareer=" << lastCareer << endl;
    save << "terrainCollisionDetailReduction=" << collisionTerrainDetailReduction << endl;
    save << "overrideDefaultDetailReduction=" << overrideDefaultDetailReduction << endl;
    save << "showFps=" << showFps << endl;
    save << "nagScreen=" << nagScreen << endl;
    save.close();
  }
}

