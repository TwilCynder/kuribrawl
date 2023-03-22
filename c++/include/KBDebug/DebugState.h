#pragma once
#include "Fighter.h"
#include <iostream>
#include <string>

class Fighter;

namespace Debug
{
    /**
     * @brief returns the name of a give fighter state (Fighter::State).  
     * (or a string indicating "not found" if this state doesn't have a name)
     * @return const std::string& 
     */
    const std::string& state_to_string(Fighter::State);
    /**
     * @brief returns a the name of a give fighter state (Fighter::State) or a null pointer if this state doesn't have a name.
     * @return const std::string* 
     */
    const std::string* state_to_string_ptr(Fighter::State);
    /**
     * @brief Logs a fighter state's (Fighter::State) name to the debug output.
     * @param state 
     */
    void log(Fighter::State state);
    /**
     * @brief Logs a fighter state's (Fighter::State) name with a frame number to the debug output.
     * @param state 
     */
    void log(Fighter::State state, int frame);
    void log(Fighter::State state, Fighter& fighter);
    /**
     * @brief Logs the current state (Fighter::State) of a Fighter to the debug output.
     * @param fighter 
     */
    void log(Fighter& fighter);
} // namespace Debug
