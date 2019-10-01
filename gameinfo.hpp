#ifndef GAMEINFO_HPP_INCLUDED
#define GAMEINFO_HPP_INCLUDED

#include "playerprofile.hpp"
#include "missioninfo.hpp"
#include "ents/playership.hpp"

struct gameInfo_t
{
  playerInfo* player;
  missionInfo_t* mission;

  //The ship
  playerShip* ship;

  //Here we store the current state
  int score;
  int credits;


};


#endif // GAMEINFO_HPP_INCLUDED
