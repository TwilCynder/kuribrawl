#pragma once
#include "KBException.h"
#include "SDL2/SDL_types.h"
#include <iostream>
#include <bitset>

/**
 * @brief Set of functions aimed at printing stuff to a debug output.
 * 
 */
namespace Debug {
    /**
     * @brief Logs a value to the debug output
     * Currently uses stdout
     * @tparam T : type of the value
     * @param value 
     */
    template <typename T>
    void log(T value){
        std::cout << value << '\n' << std::flush;
    }

    template<>
    void log (bool value);
    
    /**
     * @brief Logs a value in hex mode to the debug output
     * 
     * @tparam T : type of the value
     * @param value 
     */
    template <typename T>
    void logHex(T value){
        std::cout << std::hex << value << '\n' << std::dec << std::flush;
    }

    /**
     * @brief Logs a value as a bit sequence to the debug output
     * Relies on std::bitset .
     * @tparam T : type of the value
     * @param value 
     */
    template <typename T>
    void logBin(T value){
        std::cout << std::bitset<sizeof(T) * 8>(value) << '\n' << std::flush;
    }
}