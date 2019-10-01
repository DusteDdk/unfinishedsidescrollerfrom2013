#ifndef ENTSPAWNER_HPP_INCLUDED
#define ENTSPAWNER_HPP_INCLUDED

//This will contain whatever magic that initiates objects from id's
//It also contains the loader for the enviroment gfx
#include <vector>
#include <dirent.h>
#include "dirs.hpp"
#include "sprite.hpp"
#include "console.hpp"
#include "ents.hpp"

#include "ents/playership.hpp"
#include "ents/camlimit.hpp"
#include "ents/background.hpp"
#include "ents/astroid.hpp"
#include "ents/cargobox.hpp"
#include "ents/effector.hpp"
#include "ents/bullets.hpp"

using namespace std;

class templateItem {
  public:
    spriteClass* sprite; //This sprite is the preview (this is a preview sprite, used by all objects in editor, used in game for tiles)
    goProperties props; //This is the standard properties for said object, only used in editor
    int type; //This is the type of object, if 0 = simple tile, then the spriteid is taken from the previewsprite
};

//Loads all objects and tiles.
class objLoader {
  public:

    int numObjs; //Dangerous name? (relevant???)
    vector<int> sectionStart;
    vector<int> sectionStop;
    int numSections;
    vector<gameObject> items;
    objLoader(spriteManagerClass* sprMan);
    int objTypeToIndex(int type);
  private:
    gameObject tempObject; //Our temp game object
    bool gotName; //used to parse
    void pClear(); //clear partial parsing result
    void pPush(); //pushes gameobject back

    fdot splitCoord( string s ); //splits coords into fdot
    int strToObjId(string s);
};

//Spawns objects into map
class objSpawner {
  private:
    spriteManagerClass* sprMan;
    bool isInEditor; //if 1 = we're in the editor

  public:
    objSpawner(spriteManagerClass* s, bool inEdit);
    ~objSpawner();
    objLoader* loader;
    gameObject* create(int objType, string fileName, GLfloat x, GLfloat y, int layer, goProperties* prop); //fileName only used for tiles
    bulletManagerClass* bMan;
};

#endif // ENTSPAWNER_HPP_INCLUDED
