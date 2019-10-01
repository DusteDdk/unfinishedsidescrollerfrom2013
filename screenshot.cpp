#include "screenshot.hpp"

bool screenShot(string d, int w, int h)
{
  FILE *fscreen;

  char cName[256];
  int i = 0;
  bool found=0;
  while(!found)
  {
    sprintf(cName, "%s/screenshot_%i.tga",d.data(),i);
    fscreen = fopen(cName,"rb");
    if(fscreen==NULL)
      found=1;
    else
      fclose(fscreen);
      i++;
  }
  int nS = w * h * 3;
  GLubyte *px = new GLubyte[nS];
  if(px == NULL)
  {
    printc("Error: Couldn't allocate memory for screenshot.");
    return 0;
  }
  fscreen = fopen(cName,"wb");

  glPixelStorei(GL_PACK_ALIGNMENT,1);
  glReadPixels(0, 0, w, h, GL_BGR, GL_UNSIGNED_BYTE, px);

  unsigned char TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};
  unsigned char header[6] = {w%256,w/256,h%256,h/256,24,0};
  fwrite(TGAheader, sizeof(unsigned char), 12, fscreen);
  fwrite(header, sizeof(unsigned char), 6, fscreen);

  fwrite(px, sizeof(GLubyte), nS, fscreen);
  fclose(fscreen);
  delete [] px;
  printc("Wrote screenshot to '%s'.", cName);
  return 1;
}
