#ifndef AI_HPP_INCLUDED
#define AI_HPP_INCLUDED

#include "../../ents.hpp"

class gameObject;
class controlClass
{
  public:
    virtual void run() {};
    gameObject* c;
};

#include "playercontrol.hpp"

#endif // AI_HPP_INCLUDED
