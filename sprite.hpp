#ifndef SPRITE_HEADER
#define SPRITE_HEADER


#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <fstream>
#include <iostream>
#include <list>
#include <vector>
#include "dirs.hpp"
#include "scene.hpp"
#include "console.hpp"
#include "general.hpp"

extern dirsClass dir;

//This is how many diffrent sprites can be loaded, not the total on screen
#define MAXSPRITES 256
using namespace std;

struct spriteInfoStruct {
  //Used by sprite
  int numFrames;
  int rate;
  bool reverse, looping, playing;

  //Used by both
//  GLfloat width, height; //Size of the sprite
//  GLfloat hw, hh; //Half size, (for positioning)
  fdot size;
  fdot halfSize;
 // vector<line> collisionLines; ///To be deprecated
  line **colLines;
  int *numLines; //array, index = frame, val = lines in array

  //Used by spriteManager
  int texw, texh;// used to allocate enough space to download the texture
  int numCols, numRows; //used to generate lines
  bool hasCollisionLines;
  int id; //The ID that was returned by load() when the sprite was created
  string fileName; //Name of the .tex file describing the sprite
  GLuint tex; //The glTexture that spriteManager binds before drawing this type of sprite
  GLuint glFrames; //glDisplayLists are contenious, this points to the first of the frames in the ani


};
typedef spriteInfoStruct spriteInfo_t;


class spriteClass {
  private:
    int ticksLeft;
    spriteInfo_t** info; //Pointer to the pointer in the array
    GLfloat col[4];
  public:
    spriteClass( spriteInfo_t** spriteInfo );

    //Animation related
    int frame;
    bool playing, loop, reverse, dir; //Dir is the direction (0 meaning forward)

    void draw(); //Draws sprite, binds texture first
    void fastDraw(); //Draws sprite, expects caller to bind correct texture beforehand

    GLuint tex(); //Returns texture from spriteInfo_t** info
    int getId(); //returns the spriteid from the sprman
    string getFileName(); // returns the filename of this sprite

    void play(bool lo,bool re, int fr);
    void pause();
    void resume();
    void stop();
    void move(GLfloat x, GLfloat y); //This moves the sprite in the directions
    void position(GLfloat x, GLfloat y); //This places the top left corner of the sprite at this position
    void position(fdot p);
    void setZoom( GLfloat z );
    void rotate( GLfloat deg); //Change rotation to deg degrees
    void color(GLfloat r, GLfloat g, GLfloat b, GLfloat a);

    //Gameplay related
    int numFrames;
    GLfloat zoom;
    GLfloat rot; //Rotation of this sprite (in degrees)
    //GLfloat posx,posy;
    fdot pos;

    fdot size;
    fdot halfSize;


    //Collision lines
    vector<line> collisionLines;
    bool hasCollisionLines();
    void drawCollisionLines();
    void updateCollisionLines(); //Usally called when box-collision is true
};


class spriteManagerClass {
  private:
    int numSprites;                   //Number of sprites
    list<spriteClass*> sprites;       //Pointers to the actual sprites
    spriteInfo_t* infos[MAXSPRITES];  //When a new sprite is loaded, a pointer to it's info is added to this
    bool loadSprite(string fileName, int id); //Returns false on failure
  public:
    spriteManagerClass();
    int load(string fileName);          //Loads a sprite, returns a ID to spawn sprites of this type
    spriteClass* spawn(int id);        //Instantiates sprite of that ID, returns pointer to it.
    spriteClass* spawnByFile(string fileName); //Same as above, but will look through spriteInfos to find out if it's allready loaded, then return it.
    void genCollisionLines(int id);    //Generates collision lines for sprite ID in case they are not allready there..
    bool isSurfacePx( int x, int y, int sx, int sy,bool **bm);
    void freeGL();                     //Remove all gl stuff before we loose the context (windows thing)
    void refresh();                    //reloads the gl stuff into the same slots as they had before
    ~spriteManagerClass();
};

#endif
