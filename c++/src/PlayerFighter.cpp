#include "PlayerFighter.h"
#include "InputManager.h"
#include "Port.h"
#include "Debug.h"

PlayerFighter::PlayerFighter(Champion* model):
    Fighter(model),
    port(nullptr),
    valid_port(false),
    input_manager(std::make_unique<InputManager>(this))
{
    init_control_stick_buffer();
}


PlayerFighter::PlayerFighter(Champion* model, int x, int y):
    Fighter(model, x, y),
    port(nullptr),
    valid_port(false),
    input_manager(std::make_unique<InputManager>(this))
{
    init_control_stick_buffer();
}

PlayerFighter::~PlayerFighter(){
}

void PlayerFighter::swap_control_stick_buffer(){ 
    /*int* buffer = control_stick_buffer[1];
    control_stick_buffer[1] = control_stick_buffer[0];
    control_stick_buffer[0] = buffer;*/
}

void PlayerFighter::init_control_stick_buffer(){
    for(int i = 0; i < CONTROL_STICK_FRAME_BUFFER; i++){
        control_stick_buffer[i].x = false;
        control_stick_buffer[i].y = false;
    }
}

void PlayerFighter::update_control_stick_buffer(const Vector& current_state, const Vector& previous_state, const ControllerType::ControllerVals& vals){
    //At this point we assume the buffer is actually containing the position of the stick 2 frames ago
    if (current_state.x > vals.analogStickSmashThreshold 
        && previous_state.x < vals.analogStickSmashThreshold 
        && control_stick_buffer[0].x)
    {
        //Smash input right
        input_manager->registerInput(Input::RIGHT, port, -1, ElementType::STICK);
    } else if (current_state.x < -vals.analogStickSmashThreshold 
        && previous_state.x > -vals.analogStickSmashThreshold 
        && control_stick_buffer[0].x)
    {
        //Smash input left
        input_manager->registerInput(Input::LEFT, port, -1, ElementType::STICK);
    }

    control_stick_buffer[0].x = abs(previous_state.x < vals.analogStickThreshold);
    control_stick_buffer[0].y = abs(previous_state.y < vals.analogStickThreshold);
}

/**
 * @brief Check is the sticks of the controller used by this Fighter's Port are in a position that should lead to an action (according to the current state) and takes it.
 */
void PlayerFighter::checkStickState(){ //lots of error checks to do

    const Vector& control_stick_state = port->getControlStickState();
    const ControllerType::ControllerVals& controller_vals = port->getController()->getControllerVals();

    update_control_stick_buffer(control_stick_state, port->getControlStickPreviousState(), controller_vals);

    switch (state){
        case State::IDLE:
            if (abs(control_stick_state.x) > controller_vals.analogStickThreshold){
                if (grounded){
                    setState(State::WALK, sign(control_stick_state.x));
                }
            }
            break;
        case State::WALK:
            if (abs(control_stick_state.x) < controller_vals.analogStickThreshold){
                setState(State::IDLE);
            } else if (sign(control_stick_state.x) != facing){
                
                setState(State::WALK, -facing, 0, false);
            }
            break;
        case State::DASH:
            if (abs(control_stick_state.x) < controller_vals.analogStickThreshold){
                setState(State::DASH_STOP);
            }
            break;
        case State::DASH_START:
            if (abs(control_stick_state.x) < controller_vals.analogStickThreshold){
                setState(State::IDLE);
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
    valid_port = true;
    port = port_;
}