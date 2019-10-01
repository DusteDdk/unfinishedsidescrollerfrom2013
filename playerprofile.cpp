#include "playerprofile.hpp"
#include <stdio.h>
#include <string.h>

extern dirsClass dir;

void playerClass::loadProfiles(playerInfo* playerInfos[8])
{
  char str[16];
  fstream f;
  string p;


  //Find profiles
  for(int i=0; i < 8; i++)
  {
    playerInfos[i] = NULL;
    //Does profile exist?
    sprintf(str, "/profile%i.bin", i);
    p = dir.save + str;
    f.open(p.data());
    if(f.is_open())
    {
      playerInfos[i] = new playerInfo;
      f.read( (char *)(playerInfos[i]), sizeof(playerInfo));
      f.close();
    }
  }
}

void playerClass::saveProfile(playerInfo* Pi)
{
  //Find Available file
  char str[16];
  ofstream f;
  string p;

  bool ok=0;
  //Check if there's a fileName, because then we need not search for one
  if(Pi->file[0] != 0)
  {
    ok=1;
    p = dir.save + Pi->file;
  } else {
    for(int i=0; i < 8; i++)
    {
      //Does profile exist?
      sprintf(str, "/profile%i.bin", i);
      p = dir.save + str;

      if(!isFile(p))
      {
        ok=1;
        strcpy( Pi->file, str );
        break;
      }
    }
  }

  if(ok)
  {
    f.open(p.data(), ofstream::binary );
    if(f.is_open())
    {
      //Save
      f.write((char *)(Pi), sizeof(playerInfo));
      printc("Saving to file '%s'.",p.data());
      f.close();
    } else {
      printc("Couldn't open '%s' for writing...", p.data());
    }
  }
}

void playerClass::deleteProfile(int profileId)
{
  char str[16];
  string rmFile;
  sprintf( str, "/profile%i.bin", profileId );
  rmFile = dir.save + str;
  if( remove( rmFile.data() ) != 0 )
  {
      printc("Error: Couldn't delete file '%s'.", rmFile.c_str());
  }
}
