#ifndef settingsHeader
#define settingsHeader

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "dirs.hpp"
#include "console.hpp"
using namespace std;

class settingsClass {
  public:
    int desktopX, desktopY, desktopDepth; //Info on the users desktop before changing videomode
    int gameX, gameY;
    bool fullScreen;
    void save();
    void load();
    string file; //What file?
    int lastCareer; //Last career? Is -1 if none
    int collisionTerrainDetailReduction; //How close can points be before they are removed
    bool overrideDefaultDetailReduction; //Don't set the suggested reduction when loading map
    bool nagScreen;
    bool showFps;
    bool noFbo;
};
#endif
