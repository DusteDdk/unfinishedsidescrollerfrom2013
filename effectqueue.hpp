#ifndef EFFECTQUEUE_HPP_INCLUDED
#define EFFECTQUEUE_HPP_INCLUDED

#include <list>

#include "general.hpp"
#include "console.hpp"

#include "effects/effectresource.hpp"
#include "effects/base.hpp"
#include "effects/fxspeedup.hpp"

enum effectType { fxBase=0, fxSpeedUp=1 };
//Pointers to all the stuff that effects needs to be able to access

class effectQueueClass
{
  public:
    void queue( fdot p, int hi,int lo );
    void run(); //Run queue
    void setup( cameraControlClass* cc, sceneInfo_t* sI );
    ~effectQueueClass();

  private:
    effectResources er;
    list<effectClass*> fxqueue;
    list<effectClass*> fxactive;
};


#endif // EFFECTQUEUE_HPP_INCLUDED
