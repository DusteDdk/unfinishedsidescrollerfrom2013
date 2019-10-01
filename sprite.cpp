#include "sprite.hpp"

extern int ticks; //This contains the number of MS since last call

spriteClass::spriteClass( spriteInfo_t** spriteInfo )
{
  info = spriteInfo;
  frame = 0;
  numFrames = (**info).numFrames;
  playing = 0;
  dir = 0;
  rot=0;
  ticksLeft = (**info).rate;

  zoom = 1.0;

  col[0] = -1.0; //Don't colorize, use previous
  col[1] = 1.0;
  col[2] = 1.0;
  col[3] = 1.0;

  playing=(**info).playing;
  loop=(**info).looping;
  reverse=(**info).reverse;
  rot=0;

  //Size
  size.x = (**info).size.x;
  size.y = (**info).size.y;
  halfSize.y = (**info).halfSize.y;
  halfSize.x = (**info).halfSize.x;

  //This is just a test
  pos.x=0;
  pos.y=0;
  position(0,0);
}

void spriteClass::draw()
{
  //Bind texture and call fastDraw
  glBindTexture( GL_TEXTURE_2D, (**info).tex );
  fastDraw();
}

void spriteClass::fastDraw()
{
  //Advance animation frame
  if(playing)
  {
    ticksLeft -= ticks;
    if(ticksLeft <  1)
    {
      ticksLeft = (**info).rate;

      (dir==0) ? frame++ : frame--;

      if( frame == (**info).numFrames )
      {
        if(reverse)
        {
          frame -= 2; // Since we're not stopping on the last frame
          dir=1;
        } else {
          frame -= 1; //Stop on the last frame
          if(loop)
          {
            frame = 0; //NOTE: If support for multiple animations pr sprite, this should be replaced with the "start with" frame
          } else {
            playing=0; //Stop Playing since we reached the end and not looping and not reversing
          }
        }
      } else if(frame < 0) //(We only get here if reverse, 0 should be "start with" frame if support for multiple anis
      {
        frame=0;
        dir=0;
        if(loop)
        {
          frame++; //We allready showed frame 0
        } else {
          playing=0;
        }
      }
    }
  }

  //Call list for this frame
  if(col[0] != -1)
    glColor4fv( col );
  glPushMatrix();
    if(zoom != 1.0)
    {
      glTranslatef( (pos.x+(**info).halfSize.x*zoom), (pos.y+(**info).halfSize.y*zoom), 0 );
      glScalef( zoom, zoom, 0.0 );
    } else {
      glTranslatef( (pos.x+(**info).halfSize.x), (pos.y+(**info).halfSize.y), 0 );
    }
    glRotatef( rot, 0, 0, 1 );

    glCallList( (**info).glFrames+frame );
  glPopMatrix();

}

void spriteClass::drawCollisionLines()
{
  if(!(**info).numLines) return;
  if(!(**info).hasCollisionLines) return;

  glDisable(GL_TEXTURE_2D);
  glBegin(GL_LINES);
  //printc("%s NumLines for frame %i: %i",getFileName().data(),frame,(**info).numLines[frame]);
  /*for(int i=0; i<(**info).numLines[frame];i++)
  {
    glColor4f( 1,0,0,1 );

    fdot a,b,ar;
    a.x = (**info).colLines[frame][i].a.x;
    a.y = (**info).colLines[frame][i].a.y;
    b = this->halfSize;
    ar = rotDot(a,b,this->rot);

    glVertex2f( ar.x+pos.x, ar.y+pos.y );
    glColor4f( 1,1,0,1 );

    a.x = (**info).colLines[frame][i].b.x;
    a.y = (**info).colLines[frame][i].b.y;
    ar = rotDot(a,b,this->rot);

    glVertex2f( ar.x+pos.x, ar.y+pos.y );
  }*/

  //The real lines
  for(int i=0; i < collisionLines.size(); i++)
  {
    glVertex2f( collisionLines[i].a.x, collisionLines[i].a.y );
    glVertex2f( collisionLines[i].b.x, collisionLines[i].b.y );
  }
  //Show rotation
  GLfloat x= fCos( this->rot ) * 100 + halfSize.x;
  GLfloat y= fSin( this->rot ) * 100 + halfSize.y;
  glVertex2f( pos.x+halfSize.x, pos.y+halfSize.y);
  glVertex2f( pos.x+x, pos.y+y);

  //Show bounding box
  glColor4f(0,1,0,1);
  glVertex2f( pos.x, pos.y );
  glVertex2f( size.x+pos.x, pos.y );

  glVertex2f( size.x+pos.x, pos.y );
  glVertex2f( size.x+pos.x, size.y+pos.y );

  glVertex2f( size.x+pos.x, size.y+pos.y );
  glVertex2f( pos.x, size.y+pos.y );

  glVertex2f( pos.x, size.y+pos.y );
  glVertex2f( pos.x, pos.y );

  glColor4f(1,1,1,1);


  glEnd();
  glEnable(GL_TEXTURE_2D);
}

void spriteClass::updateCollisionLines()
{
  if(!(**info).numLines) return;
  if(!(**info).hasCollisionLines) return;
  //Fill vector with updated collines
  line t; //temp line
  collisionLines.clear();
  //Interate through lines for this frame
  for(int i=0; i<(**info).numLines[frame];i++)
  {
    t.a = rotDot( (**info).colLines[frame][i].a, halfSize, rot );
    t.b = rotDot( (**info).colLines[frame][i].b, halfSize, rot );
    t.a.x += pos.x;
    t.a.y += pos.y;
    t.b.x += pos.x;
    t.b.y += pos.y;
    collisionLines.push_back( t );
  }
}

GLuint spriteClass::tex()
{
  return( (**info).tex );
}

int spriteClass::getId()
{
  return( (**info).id );
}

bool spriteClass::hasCollisionLines()
{
  return( (**info).hasCollisionLines );
}

string spriteClass::getFileName()
{
  return( (**info).fileName );
}

void spriteClass::play(bool lo, bool re, int fr)
{
  loop=lo;
  reverse=re;
  playing=1;
  frame=fr;
}

void spriteClass::pause()
{
  playing=0;
}

void spriteClass::resume()
{
  playing=1;
}

void spriteClass::stop()
{
  playing=0;
  frame=0;
}

void spriteClass::move(GLfloat x, GLfloat y)
{
  pos.x+=x;
  pos.y+=y;
}

void spriteClass::position(GLfloat x, GLfloat y)
{
  pos.x = x;
  pos.y = y;
}

void spriteClass::position(fdot p)
{
  pos=p;
}

void spriteClass::rotate( GLfloat deg )
{
  while(deg < 0) { deg += 360.0; }
  while(deg > 360) { deg -=360.0; }
  rot = deg;
}

void spriteClass::setZoom( GLfloat z )
{
  zoom = z;
}

void spriteClass::color( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
  col[0]=r;
  col[1]=g;
  col[2]=b;
  col[3]=a;
}

// --- End of sprite class ---

// --- SpriteManager ---

spriteManagerClass::spriteManagerClass()
{
  numSprites = 0;
}

/// Load texture, Generate DisplayLists, Setup spriteInfo for the sprite, return index (of the infos vector (where to find the info))
int spriteManagerClass::load(string fileName)
{
  loadSprite(fileName, numSprites);

  //Return the index
  numSprites++;
  return(numSprites-1);
}

bool spriteManagerClass::loadSprite(string fileName, int id)
 {
  if(id > -1 && id < MAXSPRITES)
  {
   // printc("Loading sprite '%s'", fileName.data() );
    //Add spriteInfo to the list
    infos[id] = new spriteInfo_t;
    infos[id]->id = id;
    infos[id]->hasCollisionLines=0;
    //Temp stuff
    string imgFile;
    string tempFn;
    int fw,fh, cols, rows;

    SDL_Surface* img = NULL;

    //
    infos[id]->fileName = fileName;

    //Read Info
    ifstream f; //File
    string line,set,val;

    infos[id]->fileName=fileName;
    infos[id]->numRows=1;
    infos[id]->numCols=1;

    //If it's not a .tex, give some defaults
    if(fileName.substr( fileName.length()-4 )!=".tex")
    {
      imgFile = fileName;
      infos[id]->numFrames=1;
      fw=0; //This tells the rest of the code to insert the image's own dimensions
      cols=1;
      rows=1;
      infos[id]->playing=0;
    }
    else
    {
      tempFn.clear();
      tempFn = dir.data+fileName;
      f.open( tempFn.data() );

      //Read fileinfo
      if(f.is_open())
      {
        while(!f.eof())
        {
          getline(f, line);
          if(line.find('=') != string::npos)
          {
            set=line.substr(0,line.find('='));
            val=line.substr(line.find('=')+1);
            if(set=="file")
            {
              imgFile = val;
            }
            if(set=="framex")
            {
              fw=atoi(val.data());
            }
            if(set=="framey")
            {
              fh=atoi(val.data());
            }
            if(set=="frames")
            {
              infos[id]->numFrames=atoi(val.data());
            }
            if(set=="rate")
            {
              infos[id]->rate=atoi(val.data());
            }
            if(set=="cols")
            {
              cols=atoi(val.data());
              infos[id]->numCols=cols;
            }
            if(set=="rows")
            {
              rows=atoi(val.data());
              infos[id]->numRows=rows;
            }
            if(set=="playing")
            {
              infos[id]->playing=atoi(val.data());
            }
            if(set=="looping")
            {
              infos[id]->looping=atoi(val.data());
            }
            if(set=="reverse")
            {
              infos[id]->reverse=atoi(val.data());
            }
          }
        }
      } else {
        printc("Texture: Couldn't open '%s' for reading.", fileName.data());
        return(0);
      }
      f.close();
    } //.tex

    //Read texture image
    tempFn = dir.data+imgFile;
    img = IMG_Load( tempFn.data() );
    if(img == NULL)
    {
      printc("Error loading sprite image '%s' : '%s'", imgFile.data(), SDL_GetError() );
      SDL_FreeSurface( img );
      return(0);
    }

    //In case there was no info on framesize
    if(fw==0)
    {
      fw=img->w;
      fh=img->h;
    }

    //Generate texture
    glGenTextures(1, &infos[id]->tex);
    glBindTexture(GL_TEXTURE_2D, infos[id]->tex);

    GLuint glFormat;

    //If the format is 32 bpp, then it includes alpha.
    (img->format->BitsPerPixel == 32) ? glFormat = GL_RGBA : glFormat = GL_RGB;

    //Upload
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, glFormat, img->w, img->h, 0, glFormat, GL_UNSIGNED_BYTE, img->pixels);

    //Generate displaylists
    infos[id]->glFrames = glGenLists( infos[id]->numFrames );

    //Generate animations
    GLfloat txa,txb,tya,tyb; //Tex coords
    int pxw = img->w, pxh = img->h;

    GLfloat glUnitX, glUnitY, glUnitFW, glUnitFH; //For texture placement
    GLfloat xca, xcb, yca,ycb; // For coords


    xca = ((float)fw-2.0)/(-2.0);
    xcb = ((float)fw-2.0)/2.0;
    yca = ((float)fh-2.0)/(-2.0);
    ycb = ((float)fh-2.0)/2.0;

    glUnitX = 1.0/pxw;
    glUnitY = 1.0/pxh; //One pixel of the width of the source pixel, in gltexcoords
    glUnitFW = glUnitX*fw; //The width of a frame in the texture, in gltexcoords
    glUnitFH = glUnitY*fh;
    int fr=0; //Current frame

    //Set size:
    infos[id]->size.x = xcb*2.0 ;
    infos[id]->size.y = ycb*2.0;
    infos[id]->halfSize.x = xcb;
    infos[id]->halfSize.y = ycb;
    infos[id]->texw = img->w;
    infos[id]->texh = img->h;

    for(int y=0; y < rows; y++)
    {
      for(int x=0; x < cols; x++)
      {
        txa = glUnitFW * x + glUnitX;
        txb = glUnitFW * x + glUnitFW -glUnitX;
        tya = glUnitFH * y + glUnitY;
        tyb = glUnitFH * y + glUnitFH - glUnitY;

        glNewList( infos[id]->glFrames+fr, GL_COMPILE );
          glBegin( GL_QUADS );
            glTexCoord2f( txa, tya );
            glVertex2f( xca, yca );
            glTexCoord2f( txb, tya );
            glVertex2f( xcb, yca);
            glTexCoord2f( txb, tyb );
            glVertex2f( xcb, ycb);
            glTexCoord2f( txa, tyb );
            glVertex2f( xca, ycb);
          glEnd( );
        glEndList( );
        fr++;
      }
    }
  //  cout << "(" << pxw << "x" << pxh << " px) (" << infos[id]->numFrames << " frames of "<<infos[id]->width<<"x"<<infos[id]->height<<") (id: "<<id<<")" << endl;

    //Free the sdl surface
    SDL_FreeSurface( img );
    return(1);
  } else {
    printc("SpriteManager Error: Id %i is invalid. (MaxSprites is %i)", id, MAXSPRITES);
    return(0);
  }
 }

///Instantiate a sprite of type id (info from that position in the infos vector) and return the pointer
spriteClass* spriteManagerClass::spawn(int id)
{
  sprites.push_back( new spriteClass( &infos[id] ) );
  return( sprites.back() );
}

///Looks throgh spriteInfos and if the sprite is loaded, returns id, if not, loads it, then returns
spriteClass* spriteManagerClass::spawnByFile(string fileName)
{
  for(int i=0; i < numSprites; i++)
  {
    if(infos[i]->fileName == fileName)
    {
//      printc("Spawned '%s' from existing info...", fileName.data());
      return( spawn(i) );
    }
  }

  //If we're here, it couldnt find
  return( spawn(load(fileName)) );

}

//Figures out if pixel is a surface pixel
bool spriteManagerClass::isSurfacePx( int x, int y, int sx, int sy,bool **bm)
{
  if( x+1 < sx && x != 0 && y != 0 && y+1 < sy)
  {
    //TopLeft, Top Mid, TopRight, MidLeft, MidRight, BotLeft, BotMid, BotRight
    if(bm[x-1][y-1] && bm[x][y-1] && bm[x+1][y-1] && bm[x-1][y] && bm[x+1][y] && bm[x-1][y+1] && bm[x][y+1] && bm[x+1][y+1])
    {
      return(false);
    }
  }
  return(true);
}

///Generate Collision Lines for sprite (unfinished)
void spriteManagerClass::genCollisionLines(int id)
{
  if(infos[id]->hasCollisionLines)
  {
    return;
  }
  infos[id]->hasCollisionLines=1;
  int fw = infos[id]->size.x+2;
  int fh = infos[id]->size.y+2;
  int frames = infos[id]->numFrames;
  printc("Frame size for '%s' %i,%i",infos[id]->fileName.data(),fw,fh);
  //Allocate 2d array for lines
  infos[id]->colLines = new line*[frames+1];
  infos[id]->numLines = new int[frames+1];

  //Allocate bitmap 2darray for one frame.
  bool **bm = new bool*[fw]; //Initial bitmap
  bool **bmh = new bool*[fw]; //Result of hollow bitmap
  for(int i=0; i < fw; i++)
  {
    bm[i] = new bool[fh];
    bmh[i] = new bool[fh];

    for(int y=0; y < fh; y++)
    {
      bm[i][y] = 0;
      bmh[i][y] = 0;
    }
  }

  //Allocate space for the texture
  int w = infos[id]->texw;
  int h = infos[id]->texh;
  int s = w*h*4;
  GLubyte *imgdata = new GLubyte[s];
  //Download the texture to imgdate
  glBindTexture( GL_TEXTURE_2D, infos[id]->tex );
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgdata);

  int cf=0; //current frame
  //Set them at the opposite of where they should be

  for(int row=0; row < infos[id]->numRows; row++)
  {
    for(int col=0; col < infos[id]->numCols; col++)
    {
    //Figure out where that frame is:
      //Read bits to bitmap.
      for(int y=fh*row; y < fh*row+fh; y++)
      {
        int yoffset = y*w*4;
        for(int x=fw*col; x < fw*col+fw; x++)
        {
          int offset = yoffset + x*4 +3;
          if(imgdata[offset] > 63)
          {
            bm[x-fw*col][y-fh*row]=1;
          } else {
            bm[x-fw*col][y-fh*row]=0;
          }
        }
      }

      //Copy to bmh
      for(int x=0; x < fw; x++)
      {
        for(int y=0; y < fh; y++)
        {
          bmh[x][y]=bm[x][y];
        }
      }

  //Make verts
      vector<fdot> verts;
      fdot tv;
      for(int x=0; x<fw; x++)
      {
        for(int y=0; y<fh; y++)
        {
          //Start following
          if(bmh[x][y])
          {
            bool stop=0;
            int dirx=0,diry=0;
            tv.x=x;
            tv.y=y;
            verts.push_back(tv);
            bmh[x][y]=0;
            while(!stop)
            {
              //Left->Right
              if(x < fw-1 && bmh[x+1][y] && isSurfacePx( x+1,y, fw,fh, bm) )
              {
                if(dirx!=1||diry!=0)
                {
                  tv.x=x;
                  tv.y=y;
                  verts.push_back( tv );
                }
                bmh[x+1][y]=0;
                dirx=1;
                diry=0;
                x++;
              } else
              //Up->Down
              if(y < fh && bmh[x][y+1] && isSurfacePx( x,y+1, fw,fh, bm) )
              {
                if(dirx!=0||diry!=1)
                {
                  tv.x=x;
                  tv.y=y;
                  verts.push_back( tv );
                }
                bmh[x][y+1]=0;
                dirx=0;
                diry=1;
                y++;
              } else
              //Right->Left
              if(x > 0 && bmh[x-1][y] && isSurfacePx( x-1,y, fw,fh, bm) )
              {
                if(dirx!=-1||diry!=0)
                {
                  tv.x=x;
                  tv.y=y;
                  verts.push_back( tv );
                }
                bmh[x-1][y]=0;
                dirx=-1;
                diry=0;
                x--;
              } else
              //Down->Up
              if(y > 0 && bmh[x][y-1] && isSurfacePx( x,y-1, fw,fh, bm) )
              {
                if(dirx!=0||diry!=-1)
                {
                  tv.x=x;
                  tv.y=y;
                  verts.push_back( tv );
                }
                bmh[x][y-1]=0;
                dirx=0;
                diry=-1;
                y--;
              } else
              //Down>Up>Left>Right
              if(y > 0 && x < fw-1 && bmh[x+1][y-1] && isSurfacePx( x+1,y-1, fw,fh, bm) )
              {
                if(dirx!=1||diry!=-1)
                {
                  tv.x=x;
                  tv.y=y;
                  verts.push_back( tv );
                }
                bmh[x+1][y-1]=0;
                dirx=1;
                diry=-1;
                y--;
                x++;
              } else
              //Down>Up>Right>Left

              if(y > 0 && x > 0 && bmh[x-1][y-1] && isSurfacePx( x-1,y-1, fw,fh, bm) )
              {
                if(dirx!=-1||diry!=-1)
                {
                  tv.x=x;
                  tv.y=y;
                  verts.push_back( tv );
                }
                bmh[x-1][y-1]=0;
                dirx=-1;
                diry=-1;
                y--;
                x--;
              } else
              //Up>Down>Left>Right
              if(y < fh && x < fw-1 && bmh[x+1][y+1] && isSurfacePx( x+1,y+1, fw,fh, bm) )
              {
                if(dirx!=1||diry!=1)
                {
                  tv.x=x;
                  tv.y=y;
                  verts.push_back( tv );
                }
                bmh[x+1][y+1]=0;
                dirx=1;
                diry=1;
                y++;
                x++;
              } else
              //Up>Down>Right>Left
              if(y < fh && x > 0 && bmh[x-1][y+1] && isSurfacePx( x-1,y+1, fw,fh, bm) )
              {
                if(dirx!=-1||diry!=1)
                {
                  tv.x=x;
                  tv.y=y;
                  verts.push_back( tv );
                }
                bmh[x-1][y+1]=0;
                dirx=-1;
                diry=1;
                y++;
                x--;
              } else
              {
                stop=1;
              }
            }
            goto done;
          }
        }
      }
      done:;


      //Reduce Verts
      if(verts.size() > 1)
      {
        fdot prev=verts.at(0);
        int reduce=0;
        bool erased=1;


        while(erased)
        {
          erased=0;
          for(vector<fdot>::iterator it= verts.begin(); it!=verts.end();++it)
          {
            //If a line is not diffrent than the previous, remove it

            if( (it->x < prev.x+reduce && it->x > prev.x-reduce) && (it->y < prev.y+reduce && it->y > prev.y-reduce) )
            {
              if(it!=verts.end() && it!= verts.begin())
              {
                it=verts.erase(it);
                erased=1;
              }
            }
            if(it==verts.end())
            {
              break;
            }
            prev=*it;
          }
        }

        //Convert to lines
        //Make Lines
        infos[id]->colLines[cf] = new line[ verts.size()+1 ]; //+1 for the extra
        line tl;
        int cl=0;
        for(int i=0; i<verts.size(); i++)
        {
          if(cl==0)
          {
            tl.a.x=verts.at(i).x;
            tl.a.y=verts.at(i).y;
            i++;
          }
          tl.b.x=verts.at(i).x;
          tl.b.y=verts.at(i).y;
          infos[id]->colLines[cf][cl]=tl;
          tl.a.x=verts.at(i).x;
          tl.a.y=verts.at(i).y;
          cl++;
        }

        //Close the loop
        tl.a.x=verts.at(verts.size()-1).x;
        tl.a.y=verts.at(verts.size()-1).y;
        tl.b.x=verts.at(0).x;
        tl.b.y=verts.at(0).y;
        infos[id]->colLines[cf][cl]=tl;
        cl++; //inc so the numLines for the frame is correct
        //printc("[%s] Frame %i Lines %i",infos[id]->fileName.data(), cf, cl);
        infos[id]->numLines[cf] = cl;
        //cout << "Test: " << infos[id]->numLines[cf] << endl;
      } //If Verts*/

      //Jump out when there are no more frames
      cf++;
      if(cf == frames)
      {
        break; break;
      }
    }
  }

  ///TODO: when spriteinfo is deleted, we have to free colLines and numLines.

  //Free
  for(int x=0; x<fw; x++)
  {
    delete[] bm[x];
    delete[] bmh[x];
  }

  delete[] bm;
  delete[] bmh;
  delete[] imgdata;

 }

///Erase all gl stuff
void spriteManagerClass::freeGL()
{
  //Loop through the info
  for(int i=0; i < numSprites; i++)
  {
    //Free Texture
    glDeleteTextures(1, &infos[i]->tex);

    //Free Lists
    glDeleteLists( infos[i]->glFrames, infos[i]->numFrames );
  }
}

//Load everything into the same place as it was before
void spriteManagerClass::refresh()
{
  string fileName;
  //Loop through the info
  for(int i=0; i < numSprites; i++)
  {
    fileName = infos[i]->fileName;
    loadSprite(fileName, i);
  }
}

///Clean up all lists/textures, then delete all sprites, then empty the sprites vector, then empty the spriteInfo vector
spriteManagerClass::~spriteManagerClass()
{
  //Loop through sprites and delete them
  int i=0;
  for(list<spriteClass*>::iterator it = sprites.begin(); it != sprites.end(); ++it)
  {
    delete *it;
  }


  //Empty the list
  sprites.empty();

  //Free the gl stuff
  freeGL();

  //Delete all infos
  for(int i=0; i < numSprites; i++)
  {
    delete infos[i];
  }
}
