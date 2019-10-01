#include "background.hpp"
#include "dirs.hpp"
extern dirsClass dir;


void backgroundClass::load(string image)
{
  SDL_Surface *temp, *t;
  image.insert( 0, dir.data );
  temp = IMG_Load(image.data());
  if(!temp)
  {
    printc("Error: %s",SDL_GetError());
    return;
  }

  Uint32 rmask, gmask, bmask, amask;
  SDL_Rect src={0,0,TILESIZE,TILESIZE},dst={0,0,0,0};
  #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
  #else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
  #endif

 // cout << "Loading background image '"<<image<<"' ("<<temp->w<<"x"<<temp->h<<") ";

  int x=0, y=0, ts=TILESIZE, texid=0;

  int tilesx = ceil( (float)temp->w/(float)ts ), tilesy = ceil( (float)temp->h/(float)ts );
  int tiles = tilesx*tilesy;
  numTex = tiles;
 // cout << tilesx << "x"<<tilesy << " (" << tiles << " tiles)"<<endl;
//
  glGenTextures(tiles, tex);
  glEnable( GL_TEXTURE_2D );


  t = SDL_CreateRGBSurface(0, TILESIZE, TILESIZE, 32, rmask,gmask,bmask,amask);

  char str[256];
  SDL_Rect fill = { 0,0,TILESIZE, TILESIZE };
  SDL_FillRect(t, &fill, 0xff0000ff);

  for(int i=0; i < tilesx; i++)
  {
    for(int ii=0; ii < tilesy; ii++)
    {
      //find cords
       src.x = i*(TILESIZE-2);
       src.y = ii*(TILESIZE-2);

      //Blit to t
      SDL_BlitSurface(temp, &src, t, &dst);

      //upload texture
      glBindTexture(GL_TEXTURE_2D, tex[texid]);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->w, t->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, t->pixels);
      //increase
      texid++;
//       cout << "Src: " << src.x << " " << src.y << " ("<<src.w<<","<<src.h<<")"<<endl;
    }
  }
  SDL_FreeSurface( t );
  SDL_FreeSurface( temp );

  GLdouble f = 1.0/(TILESIZE-2.0);
  texid=0;
  //Generate list
  list=glGenLists(1);
  glNewList(list, GL_COMPILE);
  glColor3f(1,1,1);

  //Translate 2 px to make up for wasted pixels at the border
  //glPushMatrix();
  //glTranslatef( 1,1,0 );

  glEnable( GL_TEXTURE_2D );
  for(int i=0; i < tilesx; i++)
  {
    for(int ii=0; ii < tilesy; ii++)
    {
      glBindTexture( GL_TEXTURE_2D, tex[texid]);
      glBegin( GL_QUADS );
      glTexCoord2f( f,f );
      glVertex2f( i*(TILESIZE-2), ii*(TILESIZE-2) );
      glTexCoord2f( 1-f,f );
      glVertex2f( i*(TILESIZE-2)+(TILESIZE-2), ii*(TILESIZE-2));
      glTexCoord2f( 1-f,1-f );
      glVertex2f( i*(TILESIZE-2)+(TILESIZE-2), ii*(TILESIZE-2)+(TILESIZE-2) );
      glTexCoord2f( f,1-f );
      glVertex2f( i*(TILESIZE-2), ii*(TILESIZE-2)+(TILESIZE-2) );
      glEnd( );

//       cout << i*(TILESIZE-2) << "," << ii*(TILESIZE-2) << " -> " << i*(TILESIZE-2)+(TILESIZE-2) <<","<< ii*(TILESIZE-2)+(TILESIZE-2) << endl;
      texid++;
    }
  }
  //glPopMatrix();
  glEndList();

}

void backgroundClass::draw()
{
   glCallList(list);
}

backgroundClass::~backgroundClass()
{
  //Free textures
  glDeleteTextures(numTex, tex);

  //Free list
  glDeleteLists(list, 1);
}
