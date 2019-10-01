#ifndef CONSOLE_HPP_INCLUDED
#define CONSOLE_HPP_INCLUDED
#include <iostream>
#include <stdarg.h>

#include <vector>
#include <cmath>

#include "text.hpp"
#include "scene.hpp"
#include "control.hpp"

using namespace std;

void initConsole(glTextClass* t);
void drawConsole();
void printc(const char* format, ...);
void clarConsole();
void consoleScrollUp();
void consoleScrollDown();

#endif // CONSOLE_HPP_INCLUDED
