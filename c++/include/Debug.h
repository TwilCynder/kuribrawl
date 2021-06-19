#pragma once
#include "KBException.h"
#include "SDL2/SDL_types.h"
#include <iostream>

#define cerr std::cerr
#define cout std::cout

namespace Debug {
    template<typename T>
    void log(T value);
}