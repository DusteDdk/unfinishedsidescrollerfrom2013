#ifndef GLFX_HPP_INCLUDED
#define GLFX_HPP_INCLUDED

#define GLEW_STATIC
#include <GL/glew.h>

#include "settings.hpp"
#include "scene.hpp"
#include "general.hpp"
#include "console.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

class glFxClass
{
  public:
    glFxClass();
    void toFBO(); //render to fbo
    void fromFBO(); //render the quad
    void blackwhite(GLfloat sat);
    void dirBlur(float deg, int len, GLfloat stepSize); //blur in dir, llen px
    void motionBlur(GLfloat decay);
    void setExp(GLfloat exp);
    bool hasFBO;
  private:
    bool newFbo(GLuint *fboId, GLuint *texId);
    bool hasShader;
    void compLog(GLuint obj); //Compile log
    GLuint boxDl;
    void renderBox(); //Renders the box with the tex

    GLuint spBlackWhite; //shader program for black/white effect
    GLuint spOverExp; //shader program for setting exposure
    //Main FBO
    GLuint fboMain;
    GLuint fboMainTex;
    //The temp fbo
    GLuint fboTemp;
    GLuint fboTempTex;

    void bindFBO(GLuint f);
};

char *textFileRead(string fn);

#endif // GLFX_HPP_INCLUDED
