#ifndef LEVEL_HPP_INCLUDED
#define LEVEL_HPP_INCLUDED

#include <algorithm>
#include <vector>
#include <cmath>

#include "entspawner.hpp"
#include "settings.hpp"
#include "console.hpp"
#include "general.hpp"
#include "ents/bullets.hpp"

#define LEVELVERSION 1

//This file defines the different kinds of map entries, these are used by magic to specify gameObjects.
struct mapFileObj {
  uint8_t layer;
  int32_t x, y; //starting pos
  uint8_t type; //What kind
  goProperties prop;
};

struct mapFileTile {
  uint8_t layer;
  int32_t x, y; //starting pos
  char file[128]; //Filename of the sprite (png/jpg, or .tex if it's animated)
};

struct mapFileHeader {
  uint8_t version;
  GLfloat pOffset[2];
  uint8_t terrainReduction;
  uint32_t numTiles;
  uint32_t numObjects;
};

class mapClass {
  private:
    void sortObjs();
    spriteManagerClass* sprMan;
    void follow(int x, int y); //this follows pixel x,y in col, removing each found pixel as it goes
    void plot(int x, int y, int dx, int dy);
    //Below is the mess used internally
    vector<fdot> plotLines;
    vector< vector<fdot> > colLines;
    int h,w; //mapsize in px, absolute
    int ho,wo; //offset (to acommodate negative values, this is subtracted from h/w when accessing col array)
    bool** colb;
    bool** col;
  public:
    GLfloat pOffset[2]; //Parallax effect offset for the two background layers
    vector<line> lines; //These are the collision lines.
    mapClass(spriteManagerClass* sp);
    ~mapClass();
    vector<gameObject*> objects;
    bool loadMap( string fileName, bool inEditor ); //Loads map into objects, then sorts it
    void saveMap( string fileName ); //saves to file
    void newMap(); //Clears objects
    void makeCollisionZones(); //Generates a vector of lines
    void showColZone();
    void sortMap(); //sorts the objects array
    bulletManagerClass* bMan;
};
#endif // LEVEL_HPP_INCLUDED
