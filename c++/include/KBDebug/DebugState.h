#pragma once
#include "Fighter.h"
#include <iostream>
#include <string>

class Fighter;

namespace Debug
{
    const std::string& state_to_string(Fighter::State);
    const std::string* state_to_string_ptr(Fighter::State);
    void log(Fighter::State state);
    void log(Fighter::State state, int frame);
    void log(Fighter::State state, Fighter& fighter);
    void log(Fighter& fighter);
} // namespace Debug
