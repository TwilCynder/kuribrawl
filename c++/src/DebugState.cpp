#include "DebugState.h"
#include "Game.h"
#include <map>
#include <string>

namespace Debug {
    static const std::map<Fighter::State, std::string> state_name {
        {Fighter::State::IDLE, "Idle"},
        {Fighter::State::WALK, "Walk"},
        {Fighter::State::JUMPSQUAT, "Jumpsquat"},
        {Fighter::State::DASH, "Dash"},
        {Fighter::State::DASH_START, "Dash Start"},
        {Fighter::State::DASH_STOP, "Dash Stop"},
        {Fighter::State::DASH_TURN, "Dash turn"},
        {Fighter::State::LANDING, "Landing"}
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

void Debug::log(Fighter::State state, int frame){
    std::cout << '[' << frame << "] ";
    log(state);
}

void Debug::log(Fighter& fighter){
    log(fighter.getState(), fighter.getGame().getFrame());
}

void Debug::log(Fighter::State state, Fighter& fighter){
    log(state, fighter.getGame().getFrame());
}