#ifndef BASE_HPP_INCLUDED
#define BASE_HPP_INCLUDED

#include "effectresource.hpp"
class effectClass
{
  public:
    virtual void run() {};
    fdot pos; //Position
    bool die; //Set to 1 when finished
    virtual void init(effectResources* e) { er=e; }
    effectClass() { die=false; }
    int hiTag, loTag;
    effectResources* er;
};

#endif // BASE_HPP_INCLUDED
