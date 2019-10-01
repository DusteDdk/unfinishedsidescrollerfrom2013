#ifndef COLLISIONTEST_HPP_INCLUDED
#define COLLISIONTEST_HPP_INCLUDED

#include "sprite.hpp"
#include "general.hpp"
#include "console.hpp"

class collisionDetectionClass
{
  public:
    //Sprite to sprite collision
    bool sprites( spriteClass* sprA, spriteClass* sprB, colinfo* ci );
    //Simple box collision, ci is not filled.
    bool box( spriteClass* sA, spriteClass* sB);
};

#endif // COLLISIONTEST_HPP_INCLUDED
