#pragma once
#include "KBException.h"
#include "SDL2/SDL_types.h"
#include <iostream>
#include <bitset>
#include "Util/Logger.h"

/**
 * @brief Set of functions aimed at printing stuff to a debug output.
 * 
 */
namespace Debug {
    extern Logger out; ///< Logger using standard output
    extern Logger err; ///< Logger using standard error output

    extern SpacedLogger sout;

    /**
     * @brief Logs a value to the debug output
     * Currently uses stdout
     * @tparam T : type of the value
     * @param value 
     */
    template <typename T>
    void log(T value){
        #ifdef DEBUG
        out << value << '\n' << std::flush;
        #endif
    }

    template<>
    void log (bool value);

    template<typename T>
    void warn(T value){
        #ifdef DEBUG
        out << "\033[0;36m WARNING :  <<" << value << "\033[0m\n";
        #endif
    }
    
    /**
     * @brief Logs a value in hex mode to the debug output
     * 
     * @tparam T : type of the value
     * @param value 
     */
    template <typename T>
    void logHex(T value){
        #ifdef DEBUG
        out << std::hex << value << '\n' << std::dec << std::flush;
        #endif 
    }

    /**
     * @brief Logs a value as a bit sequence to the debug output
     * Relies on std::bitset .
     * @tparam T : type of the value
     * @param value 
     */
    template <typename T>
    void logBin(T value){
        #ifdef DEBUG
        out << std::bitset<sizeof(T) * 8>(value) << '\n' << std::flush;
        #endif
    }
}