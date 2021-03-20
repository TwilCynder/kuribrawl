#include "InputManager.h"

InputManager::InputManager(){

}

void InputManager::registerInput(Input input, Port* port, int element, ElementType element_type, int data = 0){
    inputQ.push(RegisteredInput(input, port, element, element_type, data));
}