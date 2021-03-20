#include "DebugInput.h"
#include "inputs.h"
#include <map>
#include <string>

namespace Debug {
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

void Debug::log(Input input){
    auto it = input_name.find(input);
    if (it == input_name.end()){
        std::cout << (int)input << " (enum name not found)\n" << std::flush;
    } else {
        std::cout << it->second << '\n' << std::flush;
    }
}