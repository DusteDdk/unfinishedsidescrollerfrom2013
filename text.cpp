/* ************************************************************************* *
    OldSkoolGravityGame (OSGG) Lunar Lander-like game for linux.
    Copyright (C) 2008 Jimmy Christensen ( dusted at dusted dot dk )

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * ************************************************************************* */


#include "text.hpp"

GLfloat glTextClass::getHeight(int font)
{
  return(fontInfo[font].height);
}

GLfloat glTextClass::getLength(string text, int font)
{
  GLfloat maxLen=0, len=0;
  int c;
  for(unsigned int i=0; i < text.length(); i++)
  {
    c = (unsigned int)text[i];
    len += fontInfo[font].ch[c].width;

    if(len > maxLen)
    {
      maxLen=len;
    }

    if((char)c=='\n')
    {
      len=0;
    }

  }
  return(maxLen);
}

glTextClass::glTextClass()
{
  TTF_Init();
  //Parse font-description file
  string line,set,val,tempName;

  genFontTex(dir.data+"Bandal.ttf", 40, FONT_DEFAULT);
  genFontTex(dir.data+"Bandal.ttf", 30, FONT_MEDIUM);
  genFontTex(dir.data+"Bandal.ttf", 25, FONT_MEDIUMSMALL);
  genFontTex(dir.data+"Bandal.ttf", 20, FONT_SMALL);
  genFontTex(dir.data+"Bandal.ttf", 15, FONT_TINY);
  genFontTex(dir.data+"V5Myopia.ttf", 15, FONT_BITMAP_15 );

  TTF_Quit();
}

glTextClass::~glTextClass()
{
  //Free textures
    for(int i=0; i < FONT_NUM; i++)
    {
        glDeleteTextures(1, &fontInfo[i].tex);
    }
}

void glTextClass::genFontTex(string TTFfontName, int fontSize, int font)
{
  TTF_Font *ttfFont = NULL;
  SDL_Surface *c, *t=NULL;
  Uint32 rmask, gmask, bmask, amask;
  char tempChar[2] = { 0,0 };
  int sX=0,sY=0; //Size of the rendered character
  SDL_Rect src={0,0,0,0},dst={0,0,0,0};
  SDL_Color white = { 255,255,255 };

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

  ttfFont = TTF_OpenFont( TTFfontName.data(), fontSize );

  if(!ttfFont)
  {
    cout << "Could not load font '"<<TTFfontName<<"'" << endl;
  }
  t = SDL_CreateRGBSurface(0, 512, 512, 32, rmask,gmask,bmask,amask);

    dst.x=1;
    dst.y=1;

  fontInfo[font].height=0.0;
  for(int i=32; i < 128; i++)
  {
    tempChar[0] = (char)i;

    //Render to surface
    c = TTF_RenderText_Blended(ttfFont, tempChar, white);
    SDL_SetAlpha(c, 0, 0);
    TTF_SizeUTF8(ttfFont, tempChar, &sX, &sY);

    src.x=0;
    src.y=0;
    src.w=sX;
    src.h=sY;


    if(dst.x + sX > 512)
    {
      dst.x=1;
      dst.y += sY+2;
    }

    fontInfo[font].ch[i].Xa = ( 1.0 / ( 512.0 / (float)dst.x ) );
    fontInfo[font].ch[i].Xb = ( 1.0 / ( 512.0 / ((float)dst.x+sX) ) );
    fontInfo[font].ch[i].Ya = ( 1.0 / ( 512.0 / (float)dst.y ) );
    fontInfo[font].ch[i].Yb = ( 1.0 / ( 512.0 / ((float)dst.y+sY) ) );
    fontInfo[font].ch[i].width = sX;

    if(sY > fontInfo[font].height)
    {
      fontInfo[font].height = sY;
    }

    //blit
    dst.w=sX;
    dst.h=sY;
    SDL_BlitSurface(c,&src,t,&dst);

    dst.x += sX+2; // Waste some space 1 px padding around each char

    SDL_FreeSurface(c); //Free character-surface
  }

  glGenTextures(1, &fontInfo[font].tex); //Generate a gltexture for this font

//  cout << "Font Tex is:" << fontInfo[font].tex << endl;

 glEnable( GL_TEXTURE_2D );
 glBindTexture(GL_TEXTURE_2D, fontInfo[font].tex);


  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->w, t->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, t->pixels);


  TTF_CloseFont(ttfFont); //Free the font
  SDL_FreeSurface(t); //Free text-surface
}

void glTextClass::write(string text, int font, GLfloat x, GLfloat y)
{
  int c;
  GLfloat sX,sY,posX=x, posY=y;

  glBindTexture(GL_TEXTURE_2D, fontInfo[font].tex);

    //test quad to show the map
/*    glDisable( GL_TEXTURE_2D);
        glColor3f(1,0,1);
    glBegin(GL_QUADS);
      glVertex2f( 0,0 );
      glVertex2f( 512,0 );
      glVertex2f( 512,512 );
      glVertex2f( 0, 512);
    glEnd();

    glColor3f(1,1,1);

    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    glTexCoord2f( 0,0 );
      glVertex2f( 0,0 );

    glTexCoord2f( 1,0 );
      glVertex2f( 512,0 );

    glTexCoord2f( 1,1 );
      glVertex2f( 512,512 );

    glTexCoord2f( 0,1 );
      glVertex2f( 0, 512);
      glEnd();*/

  //Draw the quads
  for(unsigned int i=0; i < text.length(); i++)
  {
    c = (unsigned int)text[i];
    sX = fontInfo[font].ch[c].width;
    sY = fontInfo[font].height;

    if((char)c=='\n')
    {
      posY += fontInfo[font].height;
      posX = x;
    }

    glBegin(GL_QUADS);
      glTexCoord2f( fontInfo[font].ch[c].Xa,fontInfo[font].ch[c].Ya ); glVertex3f( posX, posY,0);
      glTexCoord2f( fontInfo[font].ch[c].Xb,fontInfo[font].ch[c].Ya ); glVertex3f( posX+sX, posY,0);
      glTexCoord2f( fontInfo[font].ch[c].Xb,fontInfo[font].ch[c].Yb ); glVertex3f( posX+sX, posY+sY,0);
      glTexCoord2f( fontInfo[font].ch[c].Xa,fontInfo[font].ch[c].Yb ); glVertex3f( posX, posY+sY,0);
    glEnd( );
    posX += sX;
  }

}

//extremely slow and stupid, i LOVE it
void glTextClass::writeoutlined(string text, int font, GLfloat x, GLfloat y)
{
  glColor4f(0,0,0,1);
  write(text, font, x-1, y);
  write(text, font, x+1, y);
  write(text, font, x, y-1);
  write(text, font, x, y+1);
  glColor4f(1,1,1,1);
  write(text, font, x, y);
}
