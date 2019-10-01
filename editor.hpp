#ifndef EDITOR_HPP_INCLUDED
#define EDITOR_HPP_INCLUDED

#include <vector>

#include "sprite.hpp"
#include "control.hpp"
#include "text.hpp"

#include "buttonset.hpp"
#include "background.hpp"
#include "settings.hpp"
#include "dirs.hpp"
#include "label.hpp"

#include "checkbox.hpp"
#include "level.hpp"

#include "ents.hpp"

#include "entspawner.hpp"

#define GRIDSIZE 64

struct mapEnt {
  GLfloat x,y; //Starting position;
  uint8_t type; //Type of entity (0 = background, 1-> = monster/entity with ai)
  uint8_t ai; //What ai program is bound to this (if any)
};



//Implements the in-game map editor

class editorClass {
  public:
    editorClass( inputClass* input, glTextClass* glT );
    int run();

  private:
    inputClass* inp;
    glTextClass* glText;
    backgroundClass* bg;
    spriteManagerClass sprMan;
    void drawGrid(GLfloat fac, GLfloat a);
    void drawCamGuides();
    void drawMap();
    void drawUi();
    int checkClick(); //checks if any of the objects were clicked on, returns the index if, returns -1 if not
    mapClass* map;
    //vector<gameObject*> objects;
    char buf[512];
    void toggleShownLayers();
    void updatePropsChk();
    void resetInput(); //set everything back to 0

    //blablabla
    GLfloat pOffset[4]; //parallax factor (1 being front, <1=back)
    objLoader* env; //Loads objects
    objSpawner* spawner; //Spawns objects into the gameworld
    int shipSpriteId;
    string txthelp[3];
    labelClass *lblPropHiTag, *lblPropLoTag;

    //Tool states
    bool blockInp; //Blocks input to the editor (for when writing in labels);
    goProperties toolBaseProp; //Base properties to give to new objects
    goProperties* toolEditProp; //Pointer to the properties that we're editing (standard is toolBaseProp, but could be a pointer to a props in a selected obj)
    bool itemProperties; //show itemproperties
    bool mapProperties; //Show map properties
    int help; //1=small 2=big 3=manual
    GLfloat zoom;
    int workLayer; //layer to edit
    int workType; //Types we're browsing
    int selectedEntity; //-1 if nothing is selected
    bool grid; //Show grid
    bool showcol; //Show collision dots?
    bool showCamGuides;
    bool bounds; // show bounding boxes of sprites
    bool snapToGrid; //snap to grid
    bool isMapClick; //if 1, the click was not caught by any interface objects, and therefore applies to the map.
    GLfloat mx,my, sx,sy; //mousex, mousey, snapX, snapY
    //This inits a sprite with selected qualities and puts it in the vector
    void spawn();
    int spawnIndex;




    //Interface elements
    string txtWorkTypeNames[3];
    labelClass* lblFileName; //The fileName
    checkBoxClass* chkShowLayers[4]; //Individually decides which layer is drawn
    checkBoxClass* chkWorkLayer[4]; //Toggle between which layer to
    bool chkWorkLayerLast[4];
    //properties window
    checkBoxClass* chkPropBoss;
    checkBoxClass* chkPropWait;
    checkBoxClass* chkPropMustLive;
    checkBoxClass* chkPropEnemy;

    //Interface callbacks
    static void dummyCall( void* c ); //To set isMapClick=0
    static void chkWorkLayerCall( void* c );
    static void chkPropBossCall( void* c );
    static void chkPropCall( void* c ); //Updates toolEditProp according to props
    //Edit callbacks for the tag lbls
    static void lblTagEditedCall( void* c );

};

#endif // EDITOR_HPP_INCLUDED
