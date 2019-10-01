#include "collisiontest.hpp"

bool collisionDetectionClass::sprites( spriteClass* sprA, spriteClass* sprB, colinfo* ci )
{
  //Rectangle intersection (thanks to directix for his post on gamedev.net)
  if( sprA->hasCollisionLines() && sprB->hasCollisionLines() )
  {
    if( sprA->pos.y+sprA->size.y < sprB->pos.y ) return false;
    if( sprA->pos.y > sprB->pos.y+sprB->size.y ) return false;
    if( sprA->pos.x+sprA->size.x < sprB->pos.x ) return false;
    if( sprA->pos.x > sprB->pos.x+sprB->size.x ) return false;

    //If we're still here, do lineintersect
    sprA->updateCollisionLines();
    sprB->updateCollisionLines();

    for( int ia = 0; ia < sprA->collisionLines.size(); ia++)
    {
      for(int ib = 0; ib < sprB->collisionLines.size(); ib++)
      {
        //note that we just pass the ci pointer along for the caller.
        if( linesCross( &sprA->collisionLines[ia], &sprB->collisionLines[ib], &ci->position ) )
        {
          //Direction is the normal from the angle of the two middles
          fdot midA, midB, dis;
          midA.x = sprA->pos.x + sprA->halfSize.x;
          midA.y = sprA->pos.y + sprA->halfSize.y;
          midB.x = sprB->pos.x + sprB->halfSize.x;
          midB.y = sprB->pos.y + sprB->halfSize.y;
          //Find distance
          dis.x = midB.x - midA.x;
          dis.y = midB.y - midA.y;
          GLfloat tan=atan2f(dis.x,dis.y);
          ci->dir.x = -cos( tan+1.57079633 );
          ci->dir.y = -sin( tan+1.57079633 );
          return(true);
        }
      }
    }
  }

  return(false);
}

bool collisionDetectionClass::box(spriteClass* sprA, spriteClass* sprB)
{
    if( sprA->pos.y+sprA->size.y < sprB->pos.y ) return false;
    if( sprA->pos.y > sprB->pos.y+sprB->size.y ) return false;
    if( sprA->pos.x+sprA->size.x < sprB->pos.x ) return false;
    if( sprA->pos.x > sprB->pos.x+sprB->size.x ) return false;
    return(true);
}
