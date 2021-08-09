#include "InputManager.h"
#include "Debug.h"
#include "DebugInput.h"
#include "PlayerFighter.h"

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
        InputHandler handler = input_handlers[it->input];

        if (!handler || !handler(fighter, it->port, *it)){
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

namespace {
int InputHandler_Jump(PlayerFighter* fighter, Port* port, RegisteredInput& input){
    if (fighter->getGrounded()){
        fighter->setState(Fighter::State::JUMPSQUAT, 0, input.element);
    }

    return 0;
}

int InputHandler_SmashStickSide(PlayerFighter* fighter, Port* port, RegisteredInput& input){
    Fighter::State state = fighter->getState();

    int facing = (input.input == Input::LEFT) ? -1 : 1;

    if (fighter->getGrounded()){
        if (state == Fighter::State::WALK ||
            state == Fighter::State::IDLE ||
            (state == Fighter::State::DASH_START && fighter->getFacing() == -facing))
        {
            fighter->setState(Fighter::State::DASH_START, facing);
        } else if ((state == Fighter::State::DASH || state == Fighter::State::DASH_STOP) &&
            fighter->getFacing() == -facing)
        {
            fighter->setState(Fighter::State::DASH_TURN, facing);
        }
    }

    return 0;
}
}

InputManager::InputHandler InputManager::input_handlers[Input::TOTAL];

void InputManager::initInputHandlers(){
    input_handlers[Input::JUMP] = &InputHandler_Jump;
    input_handlers[Input::RIGHT] = &InputHandler_SmashStickSide;
    input_handlers[Input::LEFT] = &InputHandler_SmashStickSide;
}