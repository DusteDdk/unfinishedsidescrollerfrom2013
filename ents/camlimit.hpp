#ifndef CAMLIMIT_HPP_INCLUDED
#define CAMLIMIT_HPP_INCLUDED

#include "../ents.hpp"

class camLimit : public gameObject
{
  public:
    bool bottom;
    camLimit(bool b);
};

#endif // CAMLIMIT_HPP_INCLUDED
