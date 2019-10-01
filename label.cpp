#include "label.hpp"

labelClass::labelClass( string txt, glTextClass* glt, int f )
{
  disable=0;
  type = LABEL_SIMPLE;
  text = txt;
  gltxt = glt;
  font = f;

  txtColor[0] = -1;
  bgColor[0] = -1; //Don't draw
  bgHoverColor[0] = -1;
  bgSelectedColor[0] = -1;

  maxLen=256;
  bgLen=0;
  selected=0;
  editing=0;
  mouseDown=0;


  //To make it easy to notice if you don't move the label to it's desired position
  posx = 400;
  posy = 300;


}

void labelClass::setType( int t, void (*fp)(void* c), void* c, inputClass* i)
{
  type = t;
  functionp = fp;
  caller = c;
  inp=i;
}

void labelClass::setPos( GLfloat x, GLfloat y )
{
  posx = x;
  posy = y;
}

void labelClass::drawBg( GLfloat* color, GLfloat w, GLfloat h )
{
  if(color[0] != -1)
  {
    glDisable( GL_TEXTURE_2D );
      glColor4f( color[0],color[1],color[2],color[3] );
      glBegin( GL_QUADS );
        glVertex2f(posx, posy);
        glVertex2f(posx+w, posy);
        glVertex2f(posx+w, posy+h);
        glVertex2f(posx, posy+h);
      glEnd( );
    glEnable( GL_TEXTURE_2D );
  }
}

void labelClass::edit()
{
  editing=1;
  //Add the cursor to the text
  cursorPos = text.length();
  text +=" ";
}
void labelClass::draw()
{
  //Change position so the posx,y is the top left corner instead of middle left
  GLfloat* bg = bgColor;
  //Need size ?
  if( bgColor[0] != -1 || bgHoverColor[0] != -1 || bgSelectedColor[0] != -1 || type != LABEL_SIMPLE )
  {
    GLfloat len, height;
    height = gltxt->getHeight( font );
    //What kind of length is the background? (The actual length, or the bgLen) ?
    if(bgLen != 0)
    {
      len=bgLen;
    } else {
      len = gltxt->getLength( text, font );
    }


    //Check if the cursor is inside
    if(type != LABEL_SIMPLE)
    {
      if( (inp->mousex > posx && inp->mousex < posx+len && inp->mousey > posy && inp->mousey < posy+height) && !editing && !selected && !disable )
      {
          if(type == LABEL_CLICKABLE)
          {
            if(mouseDown && !inp->mousedown)
            {
              selected=1;
              mouseDown=0;
              functionp(caller); //Just call straight away
            } else {

              if(inp->mousedown)
              {
                mouseDown=1;
              }

            }
          } else {
            if(inp->mousedown)
            {
              selected=1;
              edit();
            }
          }
          bg=bgHoverColor;
        } else {
          bg=bgColor;
        }


      if(selected || editing)
        bg = bgSelectedColor;

      if(editing)
      {
        text.erase( cursorPos, 1 );
        if(inp->keyPress)
        {
          if(text.length() < maxLen)
          {
            text.insert( cursorPos,1,inp->keyPress);
            cursorPos++;
          }
          inp->keyPress=0;
        }

        if(inp->clickleft)
        {
          if(cursorPos > 0)
            cursorPos--;
          inp->clickleft=0;
        }
        if(inp->clickright)
        {
          if(cursorPos < text.length() )
            cursorPos++;
          inp->clickright=0;
        }

        if(inp->clickbackspace)
        {
          inp->clickbackspace=0;
          if(text.length() > cursorPos-1  )
          {
            cursorPos--;
            text.erase( cursorPos, 1 );
          }
        }

        if(inp->clickdelete)
        {
          inp->clickdelete=0;
          if(cursorPos < text.length())
          {
            text.erase( cursorPos, 1);
          }
        }

        if(inp->clickenter)
        {
          inp->clickenter=0;
          if(text.length() > 0)
          {
            editing=0;
            selected=0;
            functionp(caller);
          }
        } else {
          //Insert cursor
          text.insert( cursorPos, "|" );
        }
      }
    }

    //Draw bg ?
    drawBg( bg, len,height );

  }


  //Draw text
  glColor4f( txtColor[0],txtColor[1],txtColor[2],txtColor[3] );
  gltxt->write(text, font, posx,posy);
  glColor4f( 1,1,1,1 );

}

void labelClass::setTxtColor( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
  txtColor[0] = r;
  txtColor[1] = g;
  txtColor[2] = b;
  txtColor[3] = a;
}
void labelClass::setBgColor( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
  bgColor[0] = r;
  bgColor[1] = g;
  bgColor[2] = b;
  bgColor[3] = a;
}
void labelClass::setHoverBgColor( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
  bgHoverColor[0] = r;
  bgHoverColor[1] = g;
  bgHoverColor[2] = b;
  bgHoverColor[3] = a;
}

void labelClass::setSelectedBgColor( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
  bgSelectedColor[0] = r;
  bgSelectedColor[1] = g;
  bgSelectedColor[2] = b;
  bgSelectedColor[3] = a;
}
