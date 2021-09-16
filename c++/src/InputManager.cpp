#include "InputManager.h"
#include "Debug.h"
#include "DebugInput.h"
#include "PlayerFighter.h"
#include "macros.h"

/**
 * @brief Construct a new Input Manager object
 *
 * @param f the Fighter this InputManager will be linked to.
 */
InputManager::InputManager(PlayerFighter* f) :
    fighter(f)
{
}

/**
 * @brief Registers an input.
 * See RegisterInput for an explanation of all paramaters.
 * @param input
 * @param port
 * @param element
 * @param element_type
 * @param data
 */
void InputManager::registerInput(Input input, Port* port, int element, ElementType element_type, int data){
    inputQ.push_back(RegisteredInput(input, port, element, element_type, data));
}

/**
 * @brief Updates all registered Inputs, attempting to apply their effect.
 * Decrements the durability of all registered inputs.
 */
void InputManager::updateInputs(){
    std::deque<RegisteredInput>::iterator it;
    for (it = inputQ.begin(); it != inputQ.end();){
        if (fighter->handleInput(*it)){
            it = inputQ.erase(it);
        } else {
            ++it;
        }
    }
}

/**
 * @brief Returns the number of currently registered inputs
 *
 * @return int
 */
int InputManager::getInputsNumber() const {
    return inputQ.size();
}

