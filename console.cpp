#include "console.hpp"

/* And suddenly, it feels writing pseudo C feels right  :/ */
extern struct sceneInfo_t sceneInfo;

static vector<string> consoleBuffer;
static glTextClass* txt;
static int curLine; //Last (newest) shown line
static int numLines; // Lines we can have on console.
static GLfloat height; //text height
static int bufferSize; //how many lines do we keep in the buffer..
static int lineCount;

void clearConsole()
{
  consoleBuffer.clear();
  curLine=0;
  lineCount=0;
}

void drawConsole()
{
  /* First, overlay the grey square */
  glDisable(GL_TEXTURE_2D);
  glColor4f(0.3, 0.3, 0.3, 0.6);
  glBegin(GL_QUADS);
    glVertex2f(0,0);
    glVertex2f(sceneInfo.X, 0);
    glVertex2f(sceneInfo.X, sceneInfo.Y/2.0);
    glVertex2f(0, sceneInfo.Y/2.0);
  glEnd();
  glEnable(GL_TEXTURE_2D);

  /* Loop over text */
  int wtn=0; //Number of lines shown

  if(curLine > consoleBuffer.size()-1)
  {
    curLine=consoleBuffer.size()-1;
  }

  char lns[8];
  if(lineCount > 99999999)
    lineCount=0;

  for(int i = consoleBuffer.size()-1-curLine; (i>-1) && wtn!=numLines; i--)
  {
    wtn++;
    if(i==0)
    {
      glColor4f(1,0,0,1);
      txt->write( "_ _ _ _ _ _ _ _ _ _ _ _ _ _", FONT_TINY, 4, sceneInfo.Y/2.0-(height*wtn) );
    }
    if(curLine!=0 && wtn==1)
    {
      glColor4f(0,1,0,1);
      txt->write( " _ _ _ _ _ _ _ _ _ _ _ _ _", FONT_TINY, 4, sceneInfo.Y/2.0-(height*wtn) );
    }

    sprintf(lns, "<%i>", lineCount-wtn-curLine);
    txt->writeoutlined( consoleBuffer[i], FONT_TINY, 4, sceneInfo.Y/2.0-(height*wtn) );
    txt->writeoutlined( lns, FONT_TINY, sceneInfo.X-txt->getLength(lns,FONT_TINY), sceneInfo.Y/2.0-(height*wtn) );
  }

}

void printc(const char *format, ...)
{
  char buffer[1024];
  char idiot[1024];
  va_list args;
  va_start (args, format);
  vsprintf(buffer,format,args);
  va_end (args);
  consoleBuffer.push_back( (string)buffer );

  printf("> %s\n",buffer,consoleBuffer.size());

  if(consoleBuffer.size() > bufferSize)
  {
    vector<string>::iterator estart=consoleBuffer.begin();
    consoleBuffer.erase( estart );
  }
  lineCount++;
}

void initConsole(glTextClass* t)
{
  txt=t;
  clearConsole();
  height=txt->getHeight(FONT_TINY);
  numLines=ceil( (sceneInfo.Y/2)/height );
  bufferSize=500; //Keep # lines.
  printc("Console Initialized:");
  printc("  %i lines on screen.", numLines);
  printc("  %i lines scroll buffer.", bufferSize);
}


void consoleScrollUp()
{
  curLine+=numLines-1;
}

void consoleScrollDown()
{
  if(curLine-numLines-1 > -1)
    curLine-=numLines-1;
  else
    curLine=0;
}
