#ifndef LABELHEADER
#define LABELHEADER

/*#include <GL/gl.h>*/
#include "text.hpp"
#include "control.hpp"

using namespace std;

//NOTE: labelClass can't be both editable and selectable
enum { LABEL_SIMPLE, LABEL_EDITABLE, LABEL_CLICKABLE };

class labelClass {
  private:
    //caller->fp is called either when editing is done, or when label is selected
    void* caller;
    void (*functionp)(void* c);
    glTextClass* gltxt;
    inputClass* inp;
    int type;
    GLfloat posx, posy;
    void drawBg( GLfloat* color, GLfloat w, GLfloat h );
    GLfloat txtColor[4],bgColor[4], bgHoverColor[4], bgSelectedColor[4]; //Colors of the background quad.
    int cursorPos; //The position of the cursor in the string
    int mouseDown;
  public:
    labelClass( string txt, glTextClass* glt, int f);
    int font;
    void setType( int t, void (*fp)(void* c), void* c, inputClass* i); //Is this a simple label, a editable, or clickable one?
    void setPos( GLfloat x, GLfloat y);
    void draw();
    void setTxtColor( GLfloat r, GLfloat g, GLfloat b, GLfloat a );
    void setBgColor( GLfloat r, GLfloat g, GLfloat b, GLfloat a );
    void setHoverBgColor( GLfloat r, GLfloat g, GLfloat b, GLfloat a );
    void setSelectedBgColor( GLfloat r, GLfloat g, GLfloat b, GLfloat a );
    int  maxLen; //This only affects editing. You can't type anymore than this. (default 256)
    GLfloat bgLen; //If != 0, use this for collision/background instead of the actual length
    string text;
    bool selected; //Is true if this label is clickable and is selected (caller must unset)
    void edit(); //Set this to editing
    bool editing; //Is true if being edited (caller should not set/unset)
    bool disable; //If true, can't select
};


#endif
