#ifndef GENERAL_HPP_INCLUDED
#define GENERAL_HPP_INCLUDED

#include <GL/glew.h>
#include <iostream>
#include <GL/gl.h>

#include <cmath>
#include "console.hpp"
///General structs and stuff

struct fdot {
  GLfloat x, y;
};

struct line
{
  fdot a,b;
  fdot normal; //A point projected 90 degree from the line, used to deflect things and decide the orientation of the face
  fdot middle; //the middle of the line segment
};

///Struct with info about a collision
struct colinfo
{
  fdot position;
  fdot dir; //Surface normal
  int shealth; //starting health of object before impact
};

GLfloat abs2(GLfloat f);
bool linesCross(line *la,line *lb, fdot *col);

///RotDot returns a PointA that is rotated around PointB deg Degrees
fdot rotDot(fdot a, fdot b, float deg);

///Fast Sin/Cos using lookup tables
void initSinCos();
GLfloat fSin(float deg);
GLfloat fCos(float deg);

char *secToStr(int sec);

#endif // GENERAL_HPP_INCLUDED
