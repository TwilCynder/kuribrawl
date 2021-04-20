#include "PlayerFighter.h"
#include "InputManager.h"
#include "Port.h"

PlayerFighter::PlayerFighter(Champion* model):
    Fighter(model),
    port(nullptr),
    input_manager(std::make_unique<InputManager>(this))
{
}


PlayerFighter::PlayerFighter(Champion* model, int x, int y):
    Fighter(model, x, y),
    port(nullptr),
    input_manager(std::make_unique<InputManager>(this))
{
}

/**
 * @brief Check is the sticks of the controller used by this Fighter's Port are in a position that should lead to an action (according to the current state) and takes it.
 */
void PlayerFighter::checkStickState(){ //lots of error checks to do
    Kuribrawl::Vector control_stick_state = port->getControlStickState();
    ControllerType::ControllerVals controller_vals = port->getController()->getControllerVals();
    switch (state){
        case State::IDLE:
            if (abs(control_stick_state.x) > controller_vals.analogStickThreshold){
                if (grounded){
                    setState(State::WALK, Kuribrawl::sign(control_stick_state.x));
                }
            }
            break;
        case State::WALK:
            if (abs(control_stick_state.x) < controller_vals.analogStickThreshold){
                setState(State::IDLE);
            } else if (Kuribrawl::sign(control_stick_state.x) != facing){
                
                setState(State::WALK, -facing, 0, false);
            }
            break;
        default:
            break;
    }
}

/**
 * @brief Checks inputs that were registered by this Fighter's InputManager.
 * 
 */
void PlayerFighter::updateInputs(){
    checkStickState();
    input_manager->updateInputs();
}

/**
 * @brief returns the InputManager used by this Fighter.
 * @return InputManager* 
 */
InputManager* PlayerFighter::getInputManager() const{
    return input_manager.get();
}

/**
 * @brief Returns the Port controlling this Fighter.
 * 
 * @return Port* 
 */
Port* PlayerFighter::getPort() const {
    return port;
}

/**
 * @brief Sets the Port controlling this Fighter.
 * Makes the port active.
 * @param port_ a pointer to a Port.
 */
void PlayerFighter::setPort(Port* port_){
    port = port_;
}