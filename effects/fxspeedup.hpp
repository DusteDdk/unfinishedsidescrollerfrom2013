#ifndef FXSPEEDUP_HPP_INCLUDED
#define FXSPEEDUP_HPP_INCLUDED

#include "base.hpp"

class fxSpeedUpClass : public effectClass
{
  public:
    void run();
    void init(effectResources* e);
  private:
    GLfloat prevSpeed;
};

#endif // FXSPEEDUP_HPP_INCLUDED
