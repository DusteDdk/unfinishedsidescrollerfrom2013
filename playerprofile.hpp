#ifndef PLAYERPROFILE
#define PLAYERPROFILE

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "isfile.hpp"
#include "dirs.hpp"
#include "console.hpp"

struct playerInfo {
  char file[15]; //Name of the file
  char name[18];
  int8_t pic;
  int32_t credits;
  int8_t mission;
  int32_t score;
  uint32_t shotsFired;
  uint32_t enemiesKilled;
  uint32_t timePlayed; //Time, in seconds
  //Ship
  char shipName[18];
  uint8_t shipHull;
  uint8_t shipSlot[9];
};

class playerClass {
  public:
    void loadProfiles(playerInfo* playerInfos[8]);
    void saveProfile(playerInfo* Pi);
    void deleteProfile(int profileId);
};

#endif
