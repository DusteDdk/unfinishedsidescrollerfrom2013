#ifndef MISSIONINFO_HPP_INCLUDED
#define MISSIONINFO_HPP_INCLUDED


#include <fstream>
#include <iostream>
#include <vector>
#include "console.hpp"
#include "dirs.hpp"

extern dirsClass dir;


struct missionInfo_t {
  string missionName;
  string missionBriefing;
  string missionFile;
  int missionBounty;
  int num;
};

class missionInfoClass {
  private:
    vector<string>fileNames; //Mission file names in order
  public:
    missionInfoClass(); //Loads list of missions from data/missions/missions.txt
    bool setMission(int misNum); //Loads mission
    missionInfo_t info;
    void setNotFound();
};

#endif // MISSIONINFO_HPP_INCLUDED
