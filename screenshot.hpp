#ifndef SCREENSHOTHEADER
#define SCREENSHOTHEADER

#include "general.hpp"
/*#include <GL/gl.h>*/

#ifdef WIN32
  #include <GL/glext.h>
#endif

#include <SDL/SDL.h>
#include <stdio.h>
#include <iostream>
#include "console.hpp"
using namespace std;

bool screenShot(string d, int w, int h);

#endif
