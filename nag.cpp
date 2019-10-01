#include "nag.hpp"

extern struct sceneInfo_t sceneInfo;
extern settingsClass set;

nagScreenClass::nagScreenClass(string fileName)
{
  background = new backgroundClass;
  background->load(fileName);
}

///Fade in, then exit
void nagScreenClass::fadeIn()
{
  if(!set.nagScreen) return;
  glTranslatef( ( (sceneInfo.X-1067.0)/2.0 ),0,0 );
  fade(in);
}

void nagScreenClass::waitThenFadeOut(inputClass* inp)
{
  if(!set.nagScreen) return;
  ///Loop, check for input
  SDL_Event event;
  int frames=0;

  for(int i=8; i < 128; i++)
  {
    inp->keyStates[i]=0;
  }
  inp->mouseclick=0;
  inp->mousedown=0;

  while(1)
  {
    frames++;
    if(frames==240) goto here;
    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
        case SDL_QUIT:
          goto here;
          break;
        default:
          inp->update(&event);
          break;
      }
    }

    background->draw();
    for(int i=8; i < 128; i++)
    {
      if(inp->keyStates[i] || inp->mouseclick)
      {
        inp->keyStates[i]=0;
        inp->mouseclick=0;
        inp->mousedown=0;
        goto here;
      }
    }
    SDL_GL_SwapBuffers();
  }
  here:;
  fade(out);
  glLoadIdentity();
}

void nagScreenClass::fade(int dir)
{
  GLfloat startAlpha = (dir==in)?1.0:0.0;
  GLfloat stopAlpha = (dir==in)?0.0:1.0;
  GLfloat alpha=startAlpha;

  while( alpha!=stopAlpha )
  {
    if(startAlpha > stopAlpha)
    {
      alpha -= 0.05;
      if(alpha < stopAlpha) alpha=stopAlpha;
    } else {
      alpha += 0.05;
      if(alpha > stopAlpha) alpha=stopAlpha;
    }

    background->draw();
    glColor4f(0,0,0,alpha);
    glBegin( GL_QUADS );
      glVertex2f( 0,0 );
      glVertex2f( 1067, 0 );
      glVertex2f( 1067, sceneInfo.Y );
      glVertex2f( 0, sceneInfo.Y );
    glEnd();
    SDL_GL_SwapBuffers();
  }
}

nagScreenClass::~nagScreenClass()
{
  delete background;
}
