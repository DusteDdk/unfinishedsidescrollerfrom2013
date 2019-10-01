#include "fxspeedup.hpp"

void fxSpeedUpClass::run()
{
  if( er->sceneInfo->camX >= (GLfloat)loTag)
  {
    die=true;
    er->camControl->setSpeed( prevSpeed );
  }
}

void fxSpeedUpClass::init(effectResources* e)
{
  er=e;
  prevSpeed=er->camControl->getSpeed();
  er->camControl->setSpeed( 20.0 );
}

