#include "camcontrol.hpp"

extern struct sceneInfo_t sceneInfo;

cameraControlClass::cameraControlClass()
{
  ix = 0;
  iy = 0;
  speed=0;
  realSpeed=1;
}

void cameraControlClass::addPoint(gameObject* point)
{

  if(point->type==objCamLimitTop)
  {
    limTop.addPoint(point->p);
  } else {
    limBot.addPoint(point->p);
  }

}
void cameraControlClass::update(gameObject* player)
{
  //This is where we want to be
  if(!player)
    return;

  sceneInfo.camX += speed; //Scrollspeed

  //Dampen X speed
  if(player->velocity.x < speed)
  {
    player->velocity.x += abs2(speed-player->velocity.x)/16;
    if(player->velocity.x > speed)
      player->velocity.x=speed;
  }

  if(player->velocity.x > speed)
  {
    player->velocity.x -= abs2(speed-player->velocity.x)/16;
    if(player->velocity.x < speed)
      player->velocity.x=speed;
  }

  //Ship can't leave to the left
  if(player->p.x < sceneInfo.camX)
  {
    player->p.x=sceneInfo.camX;
    player->velocity.x=speed;
  }
  //or to the right
  if(player->p.x+player->sprite->size.x > sceneInfo.camX+sceneInfo.X)
  {
    player->p.x=sceneInfo.camX+sceneInfo.X-player->sprite->size.x;
    player->velocity.x=speed;
  }

  limTop.update(sceneInfo.camX+sceneInfo.X);
  limBot.update(sceneInfo.camX+sceneInfo.X);


  GLfloat height=limBot.ylim-limTop.ylim; //height between limits
  if(height < sceneInfo.Y)
  {
    mid=height/2.0;
    mid +=limTop.ylim;
    sceneInfo.camY=mid-sceneInfo.Y/2;
    iy=sceneInfo.camY;
  } else {
    //For a slowish movement
    GLfloat shouldbeY = player->p.y-(sceneInfo.Y/2.0);//-player->sprite->hh;
    if(iy > shouldbeY)
    {
      iy -= (iy-shouldbeY)/32;
    } else if(iy < shouldbeY)
    {
      iy += (shouldbeY-iy)/32;
    }
    sceneInfo.camY = iy;

  /// Edge ->
    if(sceneInfo.camY < limTop.ylim)
    {
      sceneInfo.camY=limTop.ylim;
      iy=sceneInfo.camY;
    } else if(sceneInfo.camY+sceneInfo.Y > limBot.ylim)
    {
      sceneInfo.camY=limBot.ylim-sceneInfo.Y;
      iy=sceneInfo.camY;
    }

  }

//  limTop.update(player->p.x);
//  limBot.update(player->p.x);
  //Stop player ship from flying outside limits
  if(player->p.y < limTop.ylim)
  {
    player->p.y=limTop.ylim;
  } else
  if(player->p.y+player->sprite->size.y > limBot.ylim)
  {
    player->p.y=limBot.ylim-player->sprite->size.y;
  }

  //Update speed (if changed)
  if(speed!=realSpeed)
  {
    if(speed < realSpeed)
    {
      speed += realSpeed/64 + 0.001;
      if(speed > realSpeed)
        speed=realSpeed;
    } else {
      speed -= speed/8 + 0.001;
      if(speed < realSpeed)
        speed=realSpeed;
    }
  }
 // printc("CamSpeed: Real: %.1f Actual: %.1f", realSpeed, speed);
}

void cameraControlClass::debug()
{
  glDisable(GL_TEXTURE_2D);
  glBegin(GL_LINE_STRIP);
    glColor4f(1,0,0,1);
    limTop.debug();
  glEnd();

  glBegin(GL_LINE_STRIP);
    glColor4f(0,1,0,1);
    limBot.debug();
  glEnd();

  glColor4f(1,1,1,1);
  //Middle
  glBegin(GL_LINES);
    glVertex2f(sceneInfo.camX,mid);
    glVertex2f(sceneInfo.camX+sceneInfo.X, mid);
  glEnd();
  glEnable(GL_TEXTURE_2D);
}

void camLimitClass::addPoint(fdot p)
{
  points.push_back(p);
}

void camLimitClass::debug()
{
  for(vector<fdot>::iterator it=points.begin(); it != points.end(); ++it)
  {
    glVertex2f( it->x, it->y );
  }
}

void camLimitClass::update(GLfloat x)
{
  fdot a,b;

  //Remove a point if it's behind a point that's allready offscreen
  for(vector<fdot>::iterator it=points.begin(); it!=points.end(); ++it)
  {
    if(it+1 != points.end())
    {
      if(it->x < sceneInfo.camX && (it+1)->x < sceneInfo.camX)
      {
        it=points.erase(it);
      } else {
        break; //Bail as soon as no more points should be deleted
      }
    }
  }

  //Find points that the ship are between and get out.
  for(vector<fdot>::iterator it=points.begin(); it!=points.end(); ++it)
  {
    if(it+1 != points.end())
    {
      if(x >= it->x && x <= (it+1)->x)
      {
        a = (*it);
        b = (*(it+1));
        break; //bail when we got what we needed
      }
    }
  }

  GLfloat slope = (b.y-a.y)/(b.x-a.x);

  ylim=a.y+(slope*(x-a.x));


  xlim=x;
}

void cameraControlClass::setSpeed(GLfloat s)
{
  realSpeed=s;
}

GLfloat cameraControlClass::getSpeed()
{
  return(realSpeed);
}
