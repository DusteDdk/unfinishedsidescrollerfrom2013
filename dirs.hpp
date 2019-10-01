#ifndef DIRSHEADER
#define DIRSHEADER

#include <sys/stat.h>
#include <dirent.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class dirsClass {
  private:
    bool checkDir(string dir);
    string userRoot;
  public:
    dirsClass();
    string data;
    string save;
    string screenshots;
    string profile; //profile file
};
#endif
