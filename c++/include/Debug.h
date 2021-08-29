#pragma once
#include "KBException.h"
#include "SDL2/SDL_types.h"
#include <iostream>
#include <bitset>

#define cerr std::cerr
#define cout std::cout

namespace Debug {
    template <typename T>
    void log(T value){
        cout << value << '\n' << std::flush;
    }
    
    template <typename T>
    void logHex(T value){
        cout << std::hex << value << '\n' << std::dec << std::flush;
    }

    template <typename T>
    void logBin(T value){
        cout << std::bitset<sizeof(T) * 8>(value) << '\n' << std::flush;
    }
}