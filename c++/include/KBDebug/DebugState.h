#pragma once
#include "Fighter.h"
#include <iostream>
#include <string>

class Fighter;

namespace Debug
{
    const std::string& state_to_string(Fighter::State);
    const std::string* state_to_string_ptr(Fighter::State);
    void log(Fighter::State state, const Fighter&);
    void printFrame(int frame);
    void logState(const Fighter&);

    void logFrame(Fighter::State state, const Fighter&);
    template <typename... Args> void logFrame(int frame, Args&&... args){
        printFrame(frame);
        out << ' ';
        log(args...);
    }
} // namespace Debug

std::ostream& operator<<(std::ostream& os, Fighter::State state);