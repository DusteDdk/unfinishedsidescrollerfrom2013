#ifndef NAG_HPP_INCLUDED
#define NAG_HPP_INCLUDED

#include <iostream>

#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL/SDL.h>

#include "background.hpp"
#include "control.hpp"
#include "scene.hpp"
#include "settings.hpp"

using namespace std;

class nagScreenClass
{
  public:
    nagScreenClass(string fileName);
    ~nagScreenClass();
    void fadeIn();
    void waitThenFadeOut(inputClass* inp);
  private:
    enum fadeDir{in,out};
    void fade(int dir); //runs the fading loop, then exits
    backgroundClass* background;
};

#endif // NAG_HPP_INCLUDED
