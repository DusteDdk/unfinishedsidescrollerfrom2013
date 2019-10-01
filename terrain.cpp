#include "terrain.hpp"

extern struct sceneInfo_t sceneInfo;

static bool lineCompFunc(line a, line b)
{
  int ax, bx;
  // First, decide which end of the lines are the first.
  if(a.a.x > a.b.x)
    ax=a.b.x;
  else
    ax=a.a.x;

  if(b.a.x > b.b.x)
    bx=b.b.x;
  else
    bx=b.a.x;

  return( ax < bx );
}

terrainClass::terrainClass(vector<line> li)
{
  printc("Terrain class initializing.. [%i lines]", li.size());
  //Add lines to internal vector.
  lines=li;
  //Sort lines by X
  sort( lines.begin(), lines.end(), lineCompFunc);

}

terrainClass::~terrainClass()
{
  printc("Terrain class shutting down..");
}

bool terrainClass::collide(vector<line> ls, colinfo *ci)
{
  fdot p;
  for(int ii=0; ii < ls.size(); ii++)
  {
    for(int i=0; i<osl.size();i++)
    {
      //Do line-intersect test
      if(linesCross( &osl[i], &ls[ii], &p ))
      {
        printc("Collision at %f,%f", p.x, p.y);

        ci->dir=osl[i].normal;

        ci->position=p;
        return(1);
      }
    }
  }
  return(0);
}

/// Adds/Removes lines, lines from x to x+sceneInfo.X is shown
void terrainClass::update(GLfloat x)
{
  //Add new lines to screen vector.
  for(int i = 0; i < lines.size(); i++)
  {
    //Check if a line is in the scene
    if(lines.at(i).a.x < x+sceneInfo.X || lines.at(i).b.x < x+sceneInfo.X)
    {
      //Add line to "onscreen"
      osl.push_back( lines.at(i) );
      //printc("Ny linje.. (Der er nu %i linjer)",osl.size());

      //Remove from this
      vector<line>::iterator it = lines.begin()+i;
      lines.erase(it);
      i--;
    } else {
      break;
    }
  }
  //Remove old lines from screen vector.
  for(int i=0; i < osl.size(); i++)
  {
    if(osl.at(i).a.x < x && osl.at(i).b.x < x)
    {
      vector<line>::iterator it = osl.begin()+i;
      osl.erase(it);
  //    printc("Fjerner linje. (Der er nu %i linjer)",osl.size());
      i--;
    } else {
      break;
    }
  }
}

///Debug function to show collision lines on screen, to test culling
void terrainClass::showColLines()
{
  glDisable( GL_TEXTURE_2D );
  glBegin(GL_LINES);
  for(int i=0; i<osl.size(); i++)
  {
    glColor3f(1,1,1);
    glVertex2f( osl.at(i).a.x, osl.at(i).a.y );
    glColor3f(1,1,0);
    glVertex2f( osl.at(i).b.x, osl.at(i).b.y );

    //show normal.
    glColor3f(1,0,0);
    glVertex2f( osl.at(i).middle.x+osl.at(i).a.x, osl.at(i).middle.y+osl.at(i).a.y );
    glColor3f(0,0,1);
    glVertex2f( osl.at(i).middle.x+osl.at(i).normal.x*10+osl.at(i).a.x, osl.at(i).middle.y+osl.at(i).normal.y*10+osl.at(i).a.y );
  }
  glEnd();
  glEnable( GL_TEXTURE_2D );
}
