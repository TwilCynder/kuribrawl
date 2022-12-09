#include "KBDebug/DebugState.h"
#include "Game.h"
#include <map>
#include <string>

namespace Debug {
    /**
     * @brief Map associating each State to a name.
     * 
     */
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

/**
 * @brief Returns the name of a state as a pointer to a string, or nullptr if no name is found.
 * 
 * @param state 
 * @return const std::string* 
 */
const std::string* Debug::state_to_string_ptr(Fighter::State state){
        auto it = state_name.find(state);
    if (it == state_name.end()){
        return nullptr;
    } else {
        return &it->second;
    }
}

/**
 * @brief Returns the name of a state as a (ref to a) string.
 * Returns a string indicating that the name was not found if no name is associated to this state.
 * @param state 
 * @return const std::string* 
 */
const std::string& Debug::state_to_string(Fighter::State state){
    auto it = state_name.find(state);
    if (it == state_name.end()){
        return stateNotFound;
    } else {
        return it->second;
    }
}

/**
 * @brief Logs a state to the debug output.
 * 
 * Uses the state name, given by state_to_string.
 * 
 * @param state 
 */
void Debug::log(Fighter::State state){
    std::cout << state_to_string(state) << '\n' << std::flush;
}

/**
 * @brief \ref Debug::log(Fighter::State) "Logs a state" to the debug output, and a frame index.  
 * 
 * @param state 
 * @param frame 
 */
void Debug::log(Fighter::State state, int frame){
    cout << '[' << frame << "] ";
    log(state);
}

/**
 * @brief Logs info about a Fighter to the debug output, aka its \ref Fighter::State "State" and name.
 * 
 * @param fighter 
 */

void Debug::log(Fighter& fighter){
    log(fighter.getState(), fighter.getGame().getFrame());
}

/**
 * @brief Logs a state and the current frame of the game in which a certain Fighter is.
 * 
 * @param state 
 * @param fighter the fighter used to get the Game.
 */
void Debug::log(Fighter::State state, Fighter& fighter){
    log(state, fighter.getGame().getFrame());
}