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

namespace {

int jump_manager(PlayerFighter* fighter, Port* port, RegisteredInput& input, jumpY type){
    Fighter::State state = fighter->getState();

	if (grounded){
		if (state == Fighter::State::JUMPSQUAT){
			
		} else {
			fighter->setState(Fighter::State::JUMPSQUAT, 0, 0 addBitValue((Uint8)type, 2) addBitValue(input.element_type, 3) addBitValue(input.element, 5));
		}
	} else {
		return air_jump()
	}	

    return 0;
}

int InputHandler_Jump(PlayerFighter* fighter, Port* port, RegisteredInput& input){
    return jump_manager(fighter, port, input, jumpY::Full);
}

int InputHandler_ShortHop(PlayerFighter* fighter, Port* port, RegisteredInput& input){
    return jump_manager(fighter, port, input, jumpY::Short);
}


int InputHandler_SmashStickSide(PlayerFighter* fighter, Port* port, RegisteredInput& input){
    Fighter::State state = fighter->getState();

    int facing = (input.input == Input::LEFT) ? -1 : 1;

    if (grounded){
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

int InputHandler_SmashStickDown(PlayerFighter* fighter, Port* port, RegisteredInput& input){


    if (!fighter->getGrounded()){

    }

    return 1;
}
}