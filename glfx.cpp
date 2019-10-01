#include <unistd.h>
#include "glfx.hpp"
extern struct sceneInfo_t sceneInfo;

extern class settingsClass set;

glFxClass::glFxClass()
{
  hasFBO=0;
  hasShader=0;

  if(set.noFbo)
  {
    printc("Option noFBO is on, not using shader/fbo effects.");
    return;
  }

  if(glewInit() != GLEW_OK)
  {
    printc("Error: Couldn't initialize glew.");
    return;
  } else {
    hasShader=1;
    printc("Glew started.");
  }

  if(GLEW_ARB_vertex_program)
  {
    printc("Vertex shader available.");
  } else {
    hasShader=0;
  }

  if(GLEW_VERSION_2_0)
  {
    printc("OpenGL version 2. supported.");
  } else {
    hasShader=0;
  }
  if(hasShader)
  {
    hasFBO=1;

    printc("Using shaders.");


    char* fsrc;
    fsrc=textFileRead("data/shaders/blackwhite.fs");
    //vsrc=textFileRead("data/shaders/msblur1.vs");

    const char* ffsrc=fsrc;

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fs,1, &ffsrc , NULL);

    free(fsrc);

    glCompileShader(fs);
    compLog(fs);

    spBlackWhite=glCreateProgram();
    glAttachShader(spBlackWhite, fs);
   // glAttachShader(sp, vs);
    glLinkProgram(spBlackWhite);
    compLog(spBlackWhite);

    fsrc=textFileRead("data/shaders/overexp.fs");
    const char* stupid=fsrc;
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs,1,&stupid,NULL);
    free(fsrc);
    glCompileShader(fs);
    spOverExp=glCreateProgram();
    glAttachShader(spOverExp, fs);
    glLinkProgram(spOverExp);
    compLog(spOverExp);
  }
  else
  {
    printc("Not using shaders.");
  }


  boxDl=glGenLists(1);
  glNewList(boxDl, GL_COMPILE);
    glBegin(GL_QUADS);
      glTexCoord2f( 0, 1 );
      glVertex2i( 0, 0 );
      glTexCoord2f( 1, 1 );
      glVertex2f( sceneInfo.X, 0 );
      glTexCoord2f( 1, 0 );
      glVertex2i( sceneInfo.X, sceneInfo.Y );
      glTexCoord2f( 0, 0 );
      glVertex2i( 0, sceneInfo.Y );
    glEnd();
  glEndList();



  newFbo( &fboMain, &fboMainTex );
  newFbo( &fboTemp, &fboTempTex );

  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

}

char* textFileRead(string fn)
{
        FILE *fp;
        char *content = NULL;

        int f, count;

        f = open(fn.data(), O_RDONLY);

        count = lseek(f, 0, SEEK_END);

        //fclose(f);


        fp = fopen(fn.data(),"rt");

        if (fp != NULL) {
                if (count > 0) {
                        content = (char *)malloc(sizeof(char) * (count+1));
                        count = fread(content,sizeof(char),count,fp);
                        content[count] = '\0';
                }
                fclose(fp);
        }


        return content;
}

bool glFxClass::newFbo(GLuint *f, GLuint *Tex)
{
  if(!hasFBO) return(false);

  glGenFramebuffersEXT(1, f); //Create fbo
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, *f); //bind it (so we can modify it)


  glGenTextures(1,Tex);
  glBindTexture(GL_TEXTURE_2D, *Tex);
//  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,  2048, 2048, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,  set.desktopX, set.desktopY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  //attach tex to fbo
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, *Tex, 0);

  if(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) == GL_FRAMEBUFFER_COMPLETE_EXT)
  {
    printc("FBO setup done.");
  } else {
    printc("Couldn't setup FBO. Error: (%i) FBO disabled.", glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT));
    hasFBO=0;
  }

}


void glFxClass::compLog(GLuint obj)
{
  int infologLength = 0;
	int maxLength;
	char* infoLog;

	if(glIsShader(obj))
		glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
	else
		glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&maxLength);

  infoLog = new char[maxLength];

	if (glIsShader(obj))
		glGetShaderInfoLog(obj, maxLength, &infologLength, infoLog);
	else
		glGetProgramInfoLog(obj, maxLength, &infologLength, infoLog);

	if (infologLength > 0)
  {
    printc("Infolog:\n%s", infoLog);
  }

  delete[] infoLog;
}


void glFxClass::toFBO()
{
  if(!hasFBO) return;

  bindFBO(fboMain);
}

void glFxClass::bindFBO(GLuint f)
{
  glPopAttrib();
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, f);
  glPushAttrib(GL_VIEWPORT_BIT);
  glViewport(0,0, set.desktopX, set.desktopY);
}


void glFxClass::fromFBO()
{
  if(!hasFBO) return;

  glPopAttrib();
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

  glColor4f(1,1,1,1);
  glBindTexture( GL_TEXTURE_2D, fboMainTex );
  renderBox();
}

void glFxClass::blackwhite(GLfloat sat)
{
  if(!hasFBO) return;
  glBindTexture(GL_TEXTURE_2D, fboMainTex);
  glUseProgram(spBlackWhite);
  GLuint loc = glGetUniformLocation(spBlackWhite, "saturation");
  glUniform1f(loc,sat);
  renderBox();
  glUseProgram(0);
}

void glFxClass::setExp(GLfloat exp)
{
  if(!hasFBO) return;
  glBindTexture(GL_TEXTURE_2D, fboMainTex);
  glUseProgram(spOverExp);
  GLuint loc = glGetUniformLocation(spOverExp, "exp");
  glUniform1f(loc,exp);
  renderBox();
  glUseProgram(0);
}

void glFxClass::dirBlur(float deg, int len, GLfloat stepSize)
{
  if(!hasFBO) return;
  glBindTexture(GL_TEXTURE_2D, fboMainTex);

  GLfloat stepX= fCos(deg);
  GLfloat stepY= fSin(deg);

  glPushMatrix();
  //First, step half the len back.
  glTranslatef( -stepX*stepSize*(len+1)/2.0, -stepY*stepSize*(len+1)/2.0, 0);
  glColor4f(1,1,1,0.80);
  for(int i=0; i < len; i++)
  {
    glTranslatef( stepX*stepSize, stepY*stepSize, 0);
    renderBox();
  }
  glPopMatrix();
  glColor4f(1,1,1,1);
}

void glFxClass::motionBlur(GLfloat decay)
{
  if(!hasFBO) return;

bindFBO(fboMain);

glColor4f(1,1,1,decay);
glBindTexture(GL_TEXTURE_2D, fboTempTex);
renderBox();


glColor4f(1,1,1,1);
bindFBO(fboTemp);
glBindTexture(GL_TEXTURE_2D, fboMainTex);
renderBox();

  bindFBO(fboMain);


}

void glFxClass::renderBox()
{
  glCallList(boxDl);
}
