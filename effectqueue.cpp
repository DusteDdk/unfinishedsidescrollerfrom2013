#include "effectqueue.hpp"

void effectQueueClass::queue(fdot p, int hi, int lo)
{
  printc("Adding Hi: %i Lo: %i at %.1fx%.1f", hi,lo,p.x,p.y);
  effectClass* tempEffect;
  switch(hi)
  {
    case fxBase:
      tempEffect=new effectClass;
    break;
    case fxSpeedUp:
      tempEffect=new fxSpeedUpClass;
    break;
    default:
      printc("Error: trying to add unspecified effect.");
      return;
    break;
  }
  tempEffect->pos=p;
  tempEffect->hiTag=hi;
  tempEffect->loTag=lo;
  fxqueue.push_back( tempEffect );
}

void effectQueueClass::setup( cameraControlClass* cc, sceneInfo_t* sI )
{
  er.camControl=cc;
  er.sceneInfo=sI;
}

effectQueueClass::~effectQueueClass()
{
  for(list<effectClass*>::iterator it=fxqueue.begin(); it!=fxqueue.end(); it++)
  {
    delete *it;
  }
  fxqueue.clear();
  for(list<effectClass*>::iterator it=fxactive.begin(); it!=fxactive.end(); it++)
  {
    delete *it;
  }
  fxactive.clear();

}

void effectQueueClass::run()
{
  //First, see if we should add any
  for(list<effectClass*>::iterator it=fxqueue.begin(); it!=fxqueue.end(); it++)
  {
    if( (*it)->pos.x < er.sceneInfo->camX+er.sceneInfo->X )
    {
      //Add
     // printc("Adding Effector H: %i L: %i", (*it)->hiTag, (*it)->loTag );
      fxactive.push_back( (*it) );
      (*it)->init( &er ); //initialize now.

      it=fxqueue.erase( it );
    } else {
      break;
    }
  }

  //check if we should remove any, run the rest
  for(list<effectClass*>::iterator it=fxactive.begin(); it!=fxactive.end(); it++)
  {
    if( (*it)->die )
    {
      printc("Killing it type %i lotag %i", (*it)->hiTag,(*it)->loTag);
      delete (*it);
      it=fxactive.erase(it);
      if(it==fxactive.end())
        break;
    } else {
      (*it)->run();
    }
  }

}

