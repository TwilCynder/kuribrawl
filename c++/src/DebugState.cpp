#include "DebugState.h"
#include <map>
#include <string>

namespace Debug {
    static const std::map<Fighter::State, std::string> state_name {
        {Fighter::State::IDLE, "Idle"},
        {Fighter::State::WALK, "Walk"},
        {Fighter::State::JUMPSQUAT, "Jumpsquat"}
    };
};

void Debug::log(Fighter::State state){
    auto it = state_name.find(state);
    if (it == state_name.end()){
        std::cout << (int)state << " (enum name not found)\n" << std::flush;
    } else {
        std::cout << it->second << '\n' << std::flush;
    }
}