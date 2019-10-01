#include "control.hpp"
#include <iostream>
using namespace std;

void inputClass::update(SDL_Event* event)
{
  switch(event->type)
  {
    case SDL_MOUSEMOTION:
      mousex = event->motion.x * sceneInfo->glUx;
      mousey = event->motion.y * sceneInfo->glUy;
    break;
    case SDL_MOUSEBUTTONDOWN:
      if(event->button.button == SDL_BUTTON_LEFT)
      {
        mousedown=1;
      } else
      if(event->button.button == SDL_BUTTON_WHEELUP)
      {
        mousewheelup=1;
      } else
      if(event->button.button == SDL_BUTTON_WHEELDOWN)
      {
        mousewheeldown=1;
      }

    break;
    case SDL_MOUSEBUTTONUP:
      if(event->button.button == SDL_BUTTON_LEFT)
      {
        mouseclick=1;
        mousedown=0;
      }
    break;

    case SDL_KEYDOWN:
      //For entering text
      if( event->key.keysym.unicode > 31 && event->key.keysym.unicode < 127)
      {
          keyPress = (char)event->key.keysym.unicode;
      }
      //For everything
      keyStates = SDL_GetKeyState( NULL );

      switch( event->key.keysym.sym )
      {
        case SDLK_RETURN:
          clickenter=1;
        break;
        case SDLK_BACKSPACE:
          clickbackspace=1;
        break;
        case SDLK_DELETE:
          clickdelete=1;
          break;
        case SDLK_LEFT:
          clickleft=1;
          left=1;
        break;
        case SDLK_RIGHT:
          clickright=1;
          right=1;
        break;
        case SDLK_UP:
          clickup=1;
          up=1;
        break;
        case SDLK_DOWN:
          clickdown=1;
          down=1;
        break;
        case SDLK_F1:
          showConsole = !showConsole;
        break;
        case SDLK_PAGEUP:
          consoleScrollUp();
        break;
        case SDLK_PAGEDOWN:
          consoleScrollDown();
        break;
        case SDLK_PAUSE:
          paused=!paused;
          (paused)?printc("Game paused."):printc("Game continue.");
        break;
        case SDLK_LCTRL:
          shoot=1;
        break;
      }
    break;
    case SDL_KEYUP:
      switch( event->key.keysym.sym )
      {
        case SDLK_RIGHT:
          right=0;
        break;
        case SDLK_LEFT:
          left=0;
        break;
        case SDLK_UP:
          up=0;
        case SDLK_DOWN:
          down=0;
        break;
        case SDLK_LCTRL:
          shoot=0;
        break;
      }
    break;


  }
}

void inputClass::setup(struct sceneInfo_t* s)
{
  SDL_EnableUNICODE (1);
  keyStates = SDL_GetKeyState( NULL );

  sceneInfo=s;
  mousedown=0;
  mousex=400;
  mousey=300;
  keyPress = 0;
  clickenter=0;
  clickbackspace=0;
  clickdelete=0;
  clickleft=0;
  clickright=0;
  clickup=0;
  clickdown=0;
  mousewheeldown=0;
  mousewheelup=0;
  showConsole=0;
  paused=0;

  up=0;
  down=0;
  left=0;
  right=0;
  shoot=0;
}
