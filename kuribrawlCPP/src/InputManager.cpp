#include "InputManager.h"
#include "Debug.h"
#include "DebugInput.h"
#include "Fighter.h"

InputManager::InputManager(Fighter* f) : 
    fighter(f)
{
}

void InputManager::registerInput(Input input, Port* port, int element, ElementType element_type, int data = 0){
    inputQ.push_back(RegisteredInput(input, port, element, element_type, data));
}

void InputManager::updateInputs(){
    std::deque<RegisteredInput>::iterator it;
    for (it = inputQ.begin(); it != inputQ.end();){
        InputHandler handler = input_handlers[it->input];
        
        if (!handler || !handler(fighter, it->port, *it)){
            it = inputQ.erase(it);
        } else {
            ++it;
        }

    }
}

int InputManager::getQSize() const {
    return inputQ.size();
}

namespace {
int InputHandler_Jump(Fighter* fighter, Port* port, RegisteredInput& input){
    Debug::log("Jumped !");
    if (fighter->getGrounded()){
        fighter->setSpeed(0, 11);
    }

    return 0;
}
}

InputManager::InputHandler InputManager::input_handlers[Input::TOTAL];

void InputManager::initInputHandlers(){
    input_handlers[Input::JUMP] = &InputHandler_Jump;
}