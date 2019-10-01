#ifndef TERRAIN_HPP_INCLUDED
#define TERRAIN_HPP_INCLUDED

#include <vector>
#include "console.hpp"
#include "level.hpp"
#include "scene.hpp"
#include "general.hpp"

/// Class to handle collision with terrain
class terrainClass
{
  public:
    terrainClass(vector<line> li);
    ~terrainClass();
    bool collide(vector<line> ls, colinfo *ci);
    void update(GLfloat x); //Add/remove lines
    void showColLines(); //Shows only the collision lines that's on screen

  private:
    vector<line> lines;
    vector<line> osl;
};

#endif // TERRAIN_HPP_INCLUDED
