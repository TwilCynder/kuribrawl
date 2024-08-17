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
        {Fighter::State::HITSTUN, "Hitstun"},
        {Fighter::State::FREEFALL, "Free Fall"}
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
 * @brief Prints a state to an ostream
 * 
 * Uses the state name, given by state_to_string.
 * @param os 
 * @param state 
 * @return std::ostream& 
 */
std::ostream& operator<<(std::ostream& os, Fighter::State state){
    return os << Debug::state_to_string(state);
};

/**
 * @brief Prints a frame index, i.e. a number in the frame index format.
 * @param frame 
 */
void Debug::printFrame(int frame)
{
    out << '[' << frame << ']';
}

/**
 * @brief Logs a state and identity info about a FIghter (ID)
 * 
 * @param state 
 * @param fighter the fighter used to get the Game.
 */
void Debug::log(Fighter::State state, const Fighter& fighter){
    out << fighter << '@' << state << '\n';
}

/**
 * @brief Logs info about a Fighter to the debug output, aka its \ref Fighter::State "State" and ID.
 * 
 * @param fighter 
 */

void Debug::logState(const Fighter& fighter){
    logFrame(fighter.getGame().getFrame(), fighter.getState(), fighter);
}

void Debug::logFrame(Fighter::State state, const Fighter & fighter)
{
    logFrame(fighter.getGame().getFrame(), state, fighter);
}
