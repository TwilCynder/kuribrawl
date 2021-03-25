#pragma once

#include "KBException.h"
#include <iostream>

#define cerr std::cerr
#define cout std::cout

namespace Debug {
    void log(std::string s);
    void log(int i);
    void log(double d);
    void log(void* p);
}