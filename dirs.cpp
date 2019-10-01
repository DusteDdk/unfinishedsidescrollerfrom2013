#include "dirs.hpp"

#ifdef WIN32
#include <windows.h>
#endif

#ifndef DATADIR
  #define DATADIR "./data/"
#endif

dirsClass::dirsClass()
{
  data = DATADIR;
  string temp;
  string dirName("/user");
  userRoot=".";
  #ifndef WIN32
    dirName = "/.sidescroller";
    if(getenv("XDG_CONFIG_HOME") != NULL)
    {
      userRoot = getenv("XDG_CONFIG_HOME");
    } else if(getenv("HOME") != NULL) {
      userRoot = getenv("HOME");
    } else {
      cout << "What kind of unix doesen't have $HOME ?" << endl;
      cout << "Using current directory for user files." << endl;
    }
  #endif

  //Check if userRoot/exist
  temp = userRoot + dirName;
  if(checkDir(temp))
  {
    save=temp;
    temp=save+"/screenshots";
    if(checkDir(temp))
    {
      screenshots=temp;
    } else {
      screenshots="./";
      cout << "Fatal: Couldn't create Directory '" << temp <<"'." << endl;
    }
  } else {
    save="./";
    cout << "Fatal: Couldn't create Directory '" << temp <<"'." << endl;
  }

  cout << "Datadir: '" << data << "' Saving in: '" << save << "'"<<endl;
}

bool dirsClass::checkDir(string dir)
{
  struct stat st;

  if( stat(dir.data(), &st) != 0)
  {
    cout << "Directory '" << dir << "' does not exist, ";
#ifdef WIN32
    if(!CreateDirectory(dir.data(), NULL))
#else
    if(mkdir(dir.data(), S_IRWXU | S_IRWXG) !=0)
#endif
    {
      cout << "could not create it." << endl;
      return(0);
    }
    cout << "created it." << endl;
  }
  return(1);
}
