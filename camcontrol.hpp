#ifndef CAMCONTROL_HPP_INCLUDED
#define CAMCONTROL_HPP_INCLUDED

#include <vector>
#include "ents.hpp"
#include "scene.hpp"
#include "general.hpp"
class gameObject;

struct camLimLine {
  GLfloat start,stop,slope;
};

class camLimitClass
{
  public:
    void addPoint(fdot p);
    void update(GLfloat x);
    GLfloat ylim;
    GLfloat xlim; //X pos, for debugging
    void debug();
  private:
    vector<fdot> points;

};

class cameraControlClass
{
  private:
    camLimitClass limTop, limBot;
    GLfloat ix,iy;
    GLfloat mid; //The middle of the limits
    GLfloat speed;
    GLfloat realSpeed; //The speed set by setspeed
  public:
    GLfloat getSpeed(); //returns realspeed
    void setSpeed( GLfloat s );
    cameraControlClass();
    void addPoint(gameObject* point);
    void update(gameObject* player);
    void debug();

};

#endif // CAMCONTROL_HPP_INCLUDED
