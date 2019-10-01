#ifndef sceneHeader
#define sceneHeader

#include "general.hpp"

struct sceneInfo_t
{
  GLfloat X,Y, glUx, glUy; //X,Y is glunits size, glUxy are for mouse

  GLfloat camX, camY; //Position of the camera
};
#endif
