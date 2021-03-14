#include "InputManager.h"

void InputManager::registerInput(Input input, Port* port, int element, ElementType element_type, int data = 0){
    RegisteredInput& registered_input = inputQ.emplace_back();
    registered_input.input = input;
    registered_input.port = port;
    registered_input.element = element;
    registered_input.element_type = element_type;
    registered_input.data = data;
}