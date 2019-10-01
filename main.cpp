#include <iostream>
#include <ctime>
#include <SDL/SDL.h>

//Linux only??
#include <GL/glew.h>

#include <GL/gl.h>


#ifdef WIN32
    typedef unsigned int uint;
    #include <windows.h>
    #include <GL/glext.h>
#endif

#define NAME "unnamed"

#include "scene.hpp"
#include "settings.hpp"
#include "menu.hpp"
#include "text.hpp"

#include "sprite.hpp"

#include "label.hpp"
#include "checkbox.hpp"
#include "screenshot.hpp"
#include "dirs.hpp"
#include "console.hpp"
#include "glfx.hpp"

#include "nag.hpp"

using namespace std;

int ticks=0;
struct sceneInfo_t sceneInfo;

dirsClass dir;
settingsClass set;
playerClass player;
glFxClass* glFx;


bool initGL(int x, int y, bool fullScreen)
{
  int fs = (fullScreen) ? SDL_FULLSCREEN : 0;
  GLfloat aspect = (float)x/(float)y;
  //Vsync
  SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);

  SDL_Surface *screen = SDL_SetVideoMode(x,y,32, SDL_OPENGL|fs);

  if(!screen)
  {
    cout << "ERROR: " << SDL_GetError() <<endl;
    return(0);
  }

  glDisable(GL_DEPTH_TEST);

  glViewport(0,0,x,y);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  sceneInfo.camX=0;
  sceneInfo.camY=0;

  sceneInfo.X = 600*aspect;
  sceneInfo.Y = 600;
  sceneInfo.glUx = sceneInfo.X/x;
  sceneInfo.glUy = sceneInfo.Y/y;

 glEnable(GL_BLEND);
 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


  glOrtho( 0, sceneInfo.X, sceneInfo.Y,0, 0,1);
  glMatrixMode(GL_MODELVIEW);

  glClearColor(0,0,0,1);

  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glEnable( GL_TEXTURE_2D );

  return(1);
}

int main(int argc, char **argv)
{

  struct timespec tsw;
  struct timespec tsl;
  tsw.tv_sec = 0;
  tsw.tv_nsec = 1666666;
  bool quit=0;
  SDL_Event event;

  srand ( time(NULL) );

  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) <0 )
  {
    cout << SDL_GetError() << endl;
  }

  SDL_WM_SetCaption(NAME, NAME);

  SDL_ShowCursor(SDL_DISABLE);

  set.desktopX = SDL_GetVideoInfo()->current_w;
  set.desktopY = SDL_GetVideoInfo()->current_h;
  set.desktopDepth = SDL_GetVideoInfo()->vfmt->BitsPerPixel;
  //load settigns
  set.load();

  if( !initGL( set.gameX, set.gameY, set.fullScreen ) )
  {
    return(1);
  }

  int lastTick=SDL_GetTicks();


  ///Vis "Intro" billedet./fade in
  nagScreenClass* nag = new nagScreenClass("/gfx/menu/intro.jpg");
  nag->fadeIn();

  glTextClass gltext;
  initConsole(&gltext);

  inputClass inp;
  inp.setup(&sceneInfo);

  spriteManagerClass* sprMan = new spriteManagerClass;

  initSinCos(); //Fill lookup tables

  menuClass menu;
  menu.setup(&inp, sprMan, &gltext);

  nag->waitThenFadeOut(&inp);
  delete nag;


  #ifdef WIN32
  //Redirect stdout to console on windows
  FILE *stream;
  stream = freopen("CON", "w", stdout);
  #endif

  glFx=new glFxClass;

  while(!quit)
  {


    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
        case SDL_QUIT:
          quit=1;
          break;
        default:
          inp.update(&event);
          break;
      }
    }

    ///Update time (ticks since last frame)
    ticks = SDL_GetTicks()-lastTick;
    lastTick=SDL_GetTicks();

    glFx->toFBO();
    glFx->blackwhite(0);
    glFx->motionBlur(100);

    glClear(GL_COLOR_BUFFER_BIT);


    ///Everything else...
    quit=menu.run();

    //The console
    if(inp.showConsole)
      drawConsole();

    //Save screenshot
    if(inp.keyPress=='s')
    {
      inp.keyPress=0;
      screenShot(dir.screenshots, set.gameX, set.gameY);
    }

    ///FPS stuff below
    static int frames=0;   //Number of frames this second
    static int fticks=0;   //"FPS ticks"
    static int lastFps=60;
    static bool wait=0;   //What is this?
    frames++;
    fticks+=ticks;
    set.showFps=1;
    if(fticks >= 1000)
    {
      if(set.showFps)
      {
        printc("Fps: %i",frames );
      }

      if(frames <59 && lastFps < 59)
      {
        //glFx->hasFBO=0;
      }

      if(frames > 61 && lastFps > 61)
        wait=1;

      lastFps=frames;
      frames=0;
      fticks=0;
    }

    glFx->fromFBO();
    SDL_GL_SwapBuffers( );

   // usleep( 1000000 );
    if(wait)
    {
      #ifdef WIN32
      Sleep(16);
      #else
      nanosleep(&tsw, &tsl);
      #endif
    }
  }

  nag = new nagScreenClass("/gfx/menu/intro.jpg");
  nag->fadeIn();
  nag->waitThenFadeOut(&inp);
  delete nag;

  SDL_ShowCursor(SDL_ENABLE);
  #ifndef WIN32
  ///Set desktop resolution back
  if(set.fullScreen) SDL_SetVideoMode(set.desktopX,set.desktopY,set.desktopDepth, SDL_OPENGL);
  #endif

  return(0);
}
