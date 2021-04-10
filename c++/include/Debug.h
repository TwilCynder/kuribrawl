#pragma once
#include "KBException.h"
#include "SDL2/SDL_types.h"
#include <iostream>

#define cerr std::cerr
#define cout std::cout

namespace Debug {
    void log(std::string s);
    void log (const char* c);
    void log(int i);
    void log(double d);
    void log(void* p);
    void log(bool b);
    void log(long l);
}