#include "InputManager.h"
#include "KBDebug/Debug.h"
#include "KBDebug/DebugMiscGameplay.h"
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
void InputManager::registerInput(Input input, int element, ElementType element_type, int data){
    inputQ.push_back(RegisteredInput(input, element, element_type, Kuribrawl::Direction::NONE, data));
}

void InputManager::registerInput(Input input, int element, ElementType element_type, Kuribrawl::Direction direction, int data)
{
    inputQ.push_back(RegisteredInput(input, element, element_type, direction, data));
}

/**
 * @brief Updates all registered Inputs, attempting to apply their effect.
 * Handles the result of the input handler if there is one for this type of input.  
 * These results can be : see PlayerFighter::InputHandlerResult.
 */
void InputManager::updateInputs(){
    using Result = PlayerFighter::InputHandlerResult;
    std::deque<RegisteredInput>::iterator it;
    for (it = inputQ.begin(); it != inputQ.end();){
        switch (fighter->handleInput(*it)){
            case Result::HANDLED:
                it = inputQ.erase(it);
                break;
            case Result::DISMISSED:
                --(it->durability);
                if (it->durability <= 0){
                    it = inputQ.erase(it);
                } else {
                    ++it;
                }
                break;
            default:
                ++it; 
                break;
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

