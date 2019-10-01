#include "general.hpp"

bool linesCross(line *la, line *lb, fdot *col)
{
	float d=(la->b.x-la->a.x)*(lb->b.y-lb->a.y)-(la->b.y-la->a.y)*(lb->b.x-lb->a.x);
	if (abs2(d)<0.001f) {return 0;}
	float AB=((la->a.y-lb->a.y)*(lb->b.x-lb->a.x)-(la->a.x-lb->a.x)*(lb->b.y-lb->a.y))/d;
	if (AB>0.0 && AB<1.0)
	{
		float CD=((la->a.y-lb->a.y)*(la->b.x-la->a.x)-(la->a.x-lb->a.x)*(la->b.y-la->a.y))/d;
		if (CD>0.0 && CD<1.0)
    {
			col->x=la->a.x+AB*(la->b.x-la->a.x);
			col->y=la->a.y+AB*(la->b.y-la->a.y);
			return 1;
      }
    }
	return 0;
}

GLfloat abs2(GLfloat f)
{
  return( (f<0)?-f:f );
}


///RotDot returns a PointA that is rotated around PointB deg Degrees
fdot rotDot(fdot a, fdot b, float deg)
{
  fdot res;
  res.x = b.x + fCos(deg)*(a.x-b.x) - fSin(deg)*(a.y-b.y);
  res.y = b.y + fSin(deg)*(a.x-b.x) + fCos(deg)*(a.y-b.y);
  return res;
}



static GLfloat fCosLookUp[36000];
static GLfloat fSinLookUp[36000];
void initSinCos()
{
  for(int deg = 0; deg < 36000; deg++)
  {
    fCosLookUp[deg]=(GLfloat)cos(0.0174532925*(float)deg/100.0);
    fSinLookUp[deg]=(GLfloat)sin(0.0174532925*(float)deg/100.0);
  }
  printc("fSinCos: Using %i KiB on Sin/Cos lookup tables.", (sizeof(float)*72000)/1024);
}
GLfloat fSin(float deg)
{
  return( fSinLookUp[(int)round(deg*100)] );
}
GLfloat fCos(float deg)
{
  return( fCosLookUp[(int)round(deg*100)] );
}

//Convert an interger value to a timestring Hours:Minutes:Seconds
char *secToStr(int sec)
{
  static char buf[16];
  static int last = -1;
  if(last!=sec)
  {
    sprintf(buf, "%i:%i:%i", sec/3600, (sec%3600)/60, sec%60);
    last=sec;
  }
  return(buf);
}
