#include "KBDebug/DebugInput.h"
#include <map>
#include <string>

namespace Debug {
    /**
     * @brief Map associating each game Input to a name.
     * 
     */
    static const std::map<Input, std::string> input_name {
        {Input::NONE, "None"},
        {Input::ATTACK, "Attack"},
        {Input::SPECIAL, "Special"},
        {Input::JUMP, "Jump"},
        {Input::SHORTHOP, "Shorthop"},
        {Input::SMASH, "Smash"},
        {Input::UP, "Up"},
        {Input::LEFT, "Left"},
        {Input::DOWN, "Down"},
        {Input::RIGHT, "Right"}
    };
};

/**
 * @brief Logs a game Input to the debug output.
 * Tried to find its name in the input_name table.
 * 
 * @param input 
 */
void Debug::log(Input input){
    auto it = input_name.find(input);
    if (it == input_name.end()){
        out << (int)input << " (enum name not found)\n" << std::flush;
    } else {
        out << it->second << '\n' << std::flush;
    }
}