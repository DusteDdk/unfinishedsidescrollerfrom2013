#ifndef EFFECTRESOURCE_HPP_INCLUDED
#define EFFECTRESOURCE_HPP_INCLUDED

#include "../general.hpp"
#include "../console.hpp"

#include "../camcontrol.hpp"
#include "../scene.hpp"

struct effectResources
{
  cameraControlClass* camControl;
  sceneInfo_t *sceneInfo;
};

#endif // EFFECTRESOURCE_HPP_INCLUDED
