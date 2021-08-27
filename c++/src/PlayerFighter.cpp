#include "PlayerFighter.h"
#include "InputManager.h"
#include "Port.h"
#include "PortOptimizationData.h"
#include "Debug.h"
#include "macros.h"
#include "Champion.h"

PlayerFighter::PlayerFighter(Game& game, Champion* model):
    Fighter(game, model),
    port(nullptr),
    valid_port(false),
    input_manager(std::make_unique<InputManager>(this))
{
    init_control_stick_buffer();
}


PlayerFighter::PlayerFighter(Game& game, Champion* model, int x, int y):
    Fighter(game, model, x, y),
    port(nullptr),
    valid_port(false),
    input_manager(std::make_unique<InputManager>(this))
{
    init_control_stick_buffer();
}

PlayerFighter::PlayerFighter(Game& game, Champion* model_, int x, int y, Port& port):
    PlayerFighter(game, model_, x, y)
{
    port.setFighter(this);
}

PlayerFighter::~PlayerFighter(){
}

/*Deleted, kept here for when such a conditional assignment will be necessary (when custom bindings will be implemented)
//use only with active ports : controller can be null if the port is inactive
Binding* PlayerFighter::getInputBinding() const {
    ControllerType* controller_type = port->getController();
    return (input_binding && input_binding->controller == controller_type) ? input_binding : controller_type->default_binding.get();
}
*/

void PlayerFighter::swap_control_stick_buffer(){
    /*int* buffer = control_stick_buffer[1];
    control_stick_buffer[1] = control_stick_buffer[0];
    control_stick_buffer[0] = buffer;*/
}

void PlayerFighter::init_control_stick_buffer(){
    for(int i = 0; i < CONTROL_STICK_FRAME_BUFFER; i++){
        control_stick_buffer[i].x = 0;
        control_stick_buffer[i].y = 0;
    }
}

void PlayerFighter::handleButtonPress(int button){
    Input input = input_binding->buttons[button];
    if (input != Input::NONE)
    input_manager->registerInput(input, port, button, ElementType::BUTTON, 0);
}

void PlayerFighter::handleTriggerPress(int trigger){
    Input input = input_binding->triggers[trigger];
    if (input != Input::NONE)
    input_manager->registerInput(input, port, trigger, ElementType::TRIGGER, 0);
}

void PlayerFighter::update_control_stick_buffer(const Vector& current_state, const Vector& previous_state, const ControllerType::ControllerVals& vals){
    //At this point we assume the buffer is actually containing the position of the stick 2 frames ago
    if (current_state.x > vals.analogStickSmashThreshold
        && previous_state.x < vals.analogStickSmashThreshold
        && control_stick_buffer[0].x != 1)
    {
        //Smash input right
		Debug::log("Smash Input Right");
        input_manager->registerInput(Input::RIGHT, port, -1, ElementType::STICK);
    } else if (current_state.x < -vals.analogStickSmashThreshold
        && previous_state.x > -vals.analogStickSmashThreshold
        && control_stick_buffer[0].x != -1)
    {
        //Smash input left
		Debug::log("Smash Input Left");
        input_manager->registerInput(Input::LEFT, port, -1, ElementType::STICK);
    }

    if (current_state.x > vals.analogStickThreshold){
        control_stick_buffer[0].x = 1;
    } else if (current_state.x < -vals.analogStickThreshold) {
        control_stick_buffer[0].x = -1;
    } else {
        control_stick_buffer[0].x = 0;
    }

}

/**
 * @brief Updates the state of the direction control of the controller
 * Which is, depending on the binding settings, either the direction given by the control stick or the dpad.
 */
void PlayerFighter::updateDirectionControlState(ControllerType::ControllerVals controller_vals){

    if (input_binding->direction_control_mode == Binding::DirectionControlMode::DPAD_ONLY){
        current_direction_control_state.x = port->getDpadStateX() * input_binding->dpadAnalogValue;
        current_direction_control_state.y = port->getDpadStateY() * input_binding->dpadAnalogValue;
    } else {
        current_direction_control_state = port->getControlStickState();
        if (
            input_binding->direction_control_mode == Binding::DirectionControlMode::BOTH &&
            (abs(current_direction_control_state.x) < controller_vals.analogStickThreshold &&abs(current_direction_control_state.y) < controller_vals.analogStickThreshold)
        )
        {
            current_direction_control_state.x = port->getDpadStateX() * input_binding->dpadAnalogValue;
            current_direction_control_state.y = port->getDpadStateY() * input_binding->dpadAnalogValue;
        }
    }
}

/**
 * @brief Check is the sticks of the controller used by this Fighter's Port are in a position that should lead to an action (according to the current state) and takes it.
 */
void PlayerFighter::checkStickState(){ //lots of error checks to do
    const ControllerType::ControllerVals& controller_vals = port->getController()->getControllerVals();

    updateDirectionControlState(controller_vals);

    update_control_stick_buffer(port->getControlStickState(), port->getControlStickPreviousState(), controller_vals);

    switch (state){
        case State::IDLE:
            if (abs(current_direction_control_state.x) > controller_vals.analogStickThreshold){
                if (grounded){
                    setState(State::WALK, sign(current_direction_control_state.x));
                } else {
					applyAirAccel(sign(current_direction_control_state.x));
				}
            }
            break;
        case State::WALK:
            if (abs(current_direction_control_state.x) < controller_vals.analogStickThreshold){
                setState(State::IDLE);
            } else if (sign(current_direction_control_state.x) != facing){

                setState(State::WALK, -facing, 0, false);
            }
            break;
        case State::DASH:
            if (current_direction_control_state.x * facing < controller_vals.analogStickThreshold){
                setState(State::DASH_STOP);
            }
            break;
        case State::DASH_START:
            if (abs(current_direction_control_state.x) < controller_vals.analogStickThreshold){
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
    if (!port) return;
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
 * @brief returns the input binding this PlayerFighter is currently using
 * 
 */

Binding* PlayerFighter::getInputBinding()const{
    return input_binding;
}

void PlayerFighter::initPortOptimizationData(PortOptimizationData& pod) const {

    pod.is_left_trigger_binding  = input_binding->triggers[0]  != Input::NONE;
    pod.is_right_trigger_binding = input_binding->triggers[1] != Input::NONE;
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

    input_binding = port->getController()->default_binding.get();
}

/**
 * @brief Unsets the current port, indicating that this PlayerFighter no longer has a port.
 * Not sure if this will be ever used but it kinda made sense to make it idk 
 */
void PlayerFighter::unsetPort(){
    valid_port = false;
    port = nullptr;
}

/**
 * @brief Returns the type of jump that should be performed if a grounded jump occured right now.
 * Considers that the fighter is in the Jumpsquat state, so state_info is the id of the element that caused the jump.
 * @return jumpY
 */
jumpY PlayerFighter::decideGroundedJumpYType() const {
	return ( 
        ((state_info >> 2) & 1) == jumpY::Full && 
        port->isElementPressed((ElementType)((state_info >> 3) & 0b11),  state_info >> 5)
    ) ? jumpY::Full : jumpY::Short;
}

int PlayerFighter::handleInput(RegisteredInput& input){
    InputHandler handler = input_handlers[input.input];
    return (handler) ? !(this->*handler)(input) : 0;
}

int PlayerFighter::jump_manager(RegisteredInput& input, jumpY type){
    if (state == Fighter::State::JUMPSQUAT){
        state_info |= 0b100;
    } else {
        setState(Fighter::State::JUMPSQUAT, 0, 0 addBitValue((Uint8)type, 2) addBitValue(input.element_type, 3) addBitValue(input.element, 5));
    }

    return 0;
}

int PlayerFighter::InputHandler_Jump(RegisteredInput& input){
    return jump_manager(input, jumpY::Full);
}

int PlayerFighter::InputHandler_ShortHop(RegisteredInput& input){
    return jump_manager(input, jumpY::Short);
}


int PlayerFighter::InputHandler_SmashStickSide(RegisteredInput& input){

    int facing = (input.input == Input::LEFT) ? -1 : 1;

    if (grounded){
        if (state == Fighter::State::WALK ||
            state == Fighter::State::IDLE ||
            (state == Fighter::State::DASH_START && facing == -facing))
        {
            setState(Fighter::State::DASH_START, facing);
        } else if ((state == Fighter::State::DASH || state == Fighter::State::DASH_STOP) &&
            facing == -facing)
        {
            setState(Fighter::State::DASH_TURN, facing);
        }
    }

    return 0;
}

int PlayerFighter::InputHandler_Attack(RegisteredInput& input){
    if (!grounded){
        startMove(*getChampion().getDefaultMove(Champion::DefaultMoves::Nair));
    }

    return 0;
}

int PlayerFighter::InputHandler_SmashStickDown(RegisteredInput& input){
    if (!grounded && speed.y < 0.0){
        speed.y = - (getChampion().val.fast_fall_speed);
    }
    return 0;
}

PlayerFighter::InputHandler PlayerFighter::input_handlers[Input::TOTAL];

void PlayerFighter::initInputHandlers(){
    input_handlers[Input::JUMP] = &InputHandler_Jump;
    input_handlers[Input::SHORTHOP] = &InputHandler_ShortHop;
    input_handlers[Input::RIGHT] = &InputHandler_SmashStickSide;
    input_handlers[Input::LEFT] = &InputHandler_SmashStickSide;
    input_handlers[Input::DOWN] = &InputHandler_SmashStickDown;
    input_handlers[Input::ATTACK] = &InputHandler_Attack;
}