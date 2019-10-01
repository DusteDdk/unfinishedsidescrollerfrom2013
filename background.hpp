#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <iostream>
#include <math.h>

#ifndef largeImageHeader
#define largeImageHeader

#include "console.hpp"

#define TILESIZE 256
using namespace std;
class backgroundClass {
  private:
    GLuint tex[16]; //Textures
    GLuint list; // The list to call
    GLuint numTex; //Allocated textures
  public:
    ~backgroundClass();
    void load(string image);
    void draw();
};

#endif
