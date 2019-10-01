#ifndef PLAYERCONTROL_HPP_INCLUDED
#define PLAYERCONTROL_HPP_INCLUDED

#include "ai.hpp"
#include "../../control.hpp"

class playerControlClass : public controlClass
{
  private:
    inputClass* inp;
  public:
    playerControlClass(inputClass* i);
    void run();
};

#endif // PLAYERCONTROL_HPP_INCLUDED
