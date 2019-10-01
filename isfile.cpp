
#include "isfile.hpp"

bool isFile(string file)
{
  struct stat st;

  if( stat(file.data(), &st) != 0)
  {
    return(0);
  }
  return(1);
}
