#include <SDL/SDL.h>
#include "scene.hpp"
#include "console.hpp"

#ifndef inputClassHeader
#define inputClassHeader
class inputClass {
  private:
    struct sceneInfo_t* sceneInfo;
  public:
    GLfloat mousex, mousey;
    bool mousedown; //holding mouse
    bool mouseclick; //after a completed click (set = 1 on mouseu, listener should set it 0)
    bool mousewheelup, mousewheeldown;
    void update(SDL_Event* event);
    void setup(struct sceneInfo_t* s);

    char keyPress; //If 0, none
    uint8_t* keyStates;
    bool clickenter, clickbackspace,clickdelete, clickleft, clickright, clickup, clickdown;
    bool showConsole;
    bool paused; //is game paused

    //To control the ship:
    bool left,right,up,down,shoot,secondary;

};
#endif
