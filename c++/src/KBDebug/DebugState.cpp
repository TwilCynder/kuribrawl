#include "KBDebug/DebugState.h"
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
        {Fighter::State::LANDING, "Landing"},
        {Fighter::State::ATTACK, "Attacking"},
        {Fighter::State::HITSTUN, "Hitstun"}
    };
};

const static std::string stateNotFound(" (enum name not found)\n");

const std::string* Debug::state_to_string_ptr(Fighter::State state){
        auto it = state_name.find(state);
    if (it == state_name.end()){
        return nullptr;
    } else {
        return &it->second;
    }
}

const std::string& Debug::state_to_string(Fighter::State state){
    auto it = state_name.find(state);
    if (it == state_name.end()){
        return stateNotFound;
    } else {
        return it->second;
    }
}

void Debug::log(Fighter::State state){
    std::cout << state_to_string(state) << '\n' << std::flush;
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