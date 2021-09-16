#include "PlayerFighter.h"
#include "InputManager.h"
#include "Port.h"
#include "PortOptimizationData.h"
#include "Debug.h"
#include "macros.h"
#include "Champion.h"
#include "controllerElements.h"

using namespace Kuribrawl;

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

void PlayerFighter::handleStickFlick(Direction direction){
    if (input_binding->second_stick != Input::NONE){
        input_manager->registerInput(input_binding->second_stick, port, 0, ElementType::STICK, 1 addBitValue((int)direction, 1));
    }
}

void PlayerFighter::update_control_stick_buffer(const Vector& current_state, const Vector& previous_state){
    //At this point we assume the buffer is actually containing the position of the stick 2 frames ago
    if (current_state.x > current_controller_vals.analogStickSmashThreshold
        && previous_state.x < current_controller_vals.analogStickSmashThreshold
        && control_stick_buffer[0].x != 1)
    {
        //Smash input right
		Debug::log("Smash Input Right");
        input_manager->registerInput(Input::RIGHT, port, -1, ElementType::STICK);
    } else if (current_state.x < -current_controller_vals.analogStickSmashThreshold
        && previous_state.x > -current_controller_vals.analogStickSmashThreshold
        && control_stick_buffer[0].x != -1)
    {
        //Smash input left
		Debug::log("Smash Input Left");
        input_manager->registerInput(Input::LEFT, port, -1, ElementType::STICK);
    }

    if (current_state.y > current_controller_vals.analogStickSmashThreshold
        && previous_state.y < current_controller_vals.analogStickSmashThreshold
        && control_stick_buffer[0].y != 1)
    {
        //Smash input right
		Debug::log("Smash Input Down");
        input_manager->registerInput(Input::DOWN, port, -1, ElementType::STICK);
    } else if (current_state.y < -current_controller_vals.analogStickSmashThreshold
        && previous_state.y > -current_controller_vals.analogStickSmashThreshold
        && control_stick_buffer[0].y != -1)
    {
        //Smash input left
		Debug::log("Smash Input UP");
        input_manager->registerInput(Input::UP, port, -1, ElementType::STICK);
    }

    if (current_state.x > current_controller_vals.analogStickThreshold){
        control_stick_buffer[0].x = 1;
    } else if (current_state.x < -current_controller_vals.analogStickThreshold) {
        control_stick_buffer[0].x = -1;
    } else {
        control_stick_buffer[0].x = 0;
    }

}

/**
 * @brief Updates the state of the direction control of the controller
 * Which is, depending on the binding settings, either the direction given by the control stick or the dpad.
 */
void PlayerFighter::updateDirectionControlState(){
    const Port::DpadState& dpad_state = port->getDpadState();
    if (input_binding->direction_control_mode == Binding::DirectionControlMode::DPAD_ONLY){
        current_direction_control_state.x = dpad_state.x * input_binding->dpadAnalogValue;
        current_direction_control_state.y = dpad_state.y * input_binding->dpadAnalogValue;
    } else {
        current_direction_control_state = port->getControlStickState().current_state;
        if (
            input_binding->direction_control_mode == Binding::DirectionControlMode::BOTH &&
            (abs(current_direction_control_state.x) < current_controller_vals.analogStickThreshold &&abs(current_direction_control_state.y) < current_controller_vals.analogStickThreshold)
        )
        {
            current_direction_control_state.x = dpad_state.x * input_binding->dpadAnalogValue;
            current_direction_control_state.y = dpad_state.y * input_binding->dpadAnalogValue;
        }
    }
}

Direction PlayerFighter::getDirection4(const Kuribrawl::Vector& stick_state) const{
    return Kuribrawl::getDirection4(stick_state, current_controller_vals.analogStickThreshold);
}

DirectionIG PlayerFighter::getDirection4IG(const Kuribrawl::Vector& stick_state) const{
    return Kuribrawl::getDirection4IG(stick_state, current_controller_vals.analogStickThreshold, facing);
}

Direction PlayerFighter::getControlDirection4() const {
    return getDirection4(current_direction_control_state);
}

DirectionIG PlayerFighter::getControlDirection4IG() const {
    return getDirection4IG(current_direction_control_state);
}

/**
 * @brief Check is the sticks of the controller used by this Fighter's Port are in a position that should lead to an action (according to the current state) and takes it.
 */
void PlayerFighter::checkStickState(){ //lots of error checks to do
    if (!valid_port) return;
    const ControllerType::ControllerVals& controller_vals = port->getController()->getControllerVals();

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
        case State::ATTACK:
            if (!grounded) applyAirAccel(sign(current_direction_control_state.x));
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
void PlayerFighter::updateInputsStates(){
    if (!valid_port) return;
    updateDirectionControlState();
    update_control_stick_buffer(port->getControlStickState().current_state, port->getControlStickState().previous_state);

    const Port::TriggerState& left_trigger = port->getLeftTriggerState();
    if (left_trigger.current_state >= current_controller_vals.analogTriggerThreshold && left_trigger.previous_state < current_controller_vals.analogTriggerThreshold){
        handleTriggerPress(TRIGGER_LEFT);
    }
    const Port::TriggerState& right_trigger = port->getRightTriggerState();
    if (right_trigger.current_state >= current_controller_vals.analogTriggerThreshold && right_trigger.previous_state < current_controller_vals.analogTriggerThreshold){
        handleTriggerPress(TRIGGER_RIGHT);
    }

    const Port::StickState& secondary_stick = port->getSecondaryStickState();
    if (secondary_stick.current_state.x > current_controller_vals.analogStickThreshold && secondary_stick.previous_state.x < current_controller_vals.analogStickThreshold){
        handleStickFlick(Direction::RIGHT);
    } else if (secondary_stick.current_state.x < -current_controller_vals.analogStickThreshold && secondary_stick.previous_state.x > -current_controller_vals.analogStickThreshold){
        handleStickFlick(Direction::LEFT);
    } 

    if (secondary_stick.current_state.y > current_controller_vals.analogStickThreshold && secondary_stick.previous_state.y < current_controller_vals.analogStickThreshold){
        handleStickFlick(Direction::DOWN);
    } else if (secondary_stick.current_state.y < -current_controller_vals.analogStickThreshold && secondary_stick.previous_state.y > -current_controller_vals.analogStickThreshold){
        handleStickFlick(Direction::UP);
    } 
}

/**
 * @brief Checks inputs that were registered by this Fighter's InputManager.
 *
 */
void PlayerFighter::resolveInputs(){
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

    pod.read_left_trigger  = input_binding->triggers[0] != Input::NONE;
    pod.read_right_trigger = input_binding->triggers[1] != Input::NONE;
    pod.read_dpad = input_binding->direction_control_mode != Binding::DirectionControlMode::STICK_ONLY;
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
    current_controller_vals = port->getController()->getControllerVals();
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
        (((state_info >> 2) & 1) == 0) && 
        port->isElementPressed((ElementType)((state_info >> 3) & 0b11),  state_info >> 5)
    ) ? jumpY::Full : jumpY::Short;
}

/**
 * @brief Returns the type of jump that should be performed if a grounded jump occured right now.
 * @return jumpX
 */
jumpX PlayerFighter::decideJumpXType() const {
    int8_t orientation = sign(current_direction_control_state.x);
    return (abs(current_direction_control_state.x) > port->getController()->getControllerVals().analogStickThreshold) ?
        (orientation == facing) ? jumpX::Forward : jumpX::Backwards :
        jumpX::Normal;
}

int PlayerFighter::handleInput(RegisteredInput& input){
    InputHandler handler = input_handlers[input.input];
    return (handler) ? !(this->*handler)(input) : 0;
}

int PlayerFighter::jump_manager(RegisteredInput& input, jumpY type){
    if (grounded){
        if (state == Fighter::State::JUMPSQUAT){
            state_info |= 0b100;
        } else {
            setState(Fighter::State::JUMPSQUAT, 0, 0 addBitValue((Uint8)(type == jumpY::Short) ? 1 : 0, 2) addBitValue(input.element_type, 3) addBitValue(input.element, 5));
        }
    } else {
        return air_jump();
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

    int side = (input.input == Input::LEFT) ? -1 : 1;

    if (grounded){
        if (state == Fighter::State::WALK ||
            state == Fighter::State::IDLE ||
            state == Fighter::State::LANDING ||
            (state == Fighter::State::DASH_START && side == -facing))
        {
            setState(Fighter::State::DASH_START, side);
        } else if ((state == Fighter::State::DASH || state == Fighter::State::DASH_STOP) &&
            side == -facing)
        {
            setState(Fighter::State::DASH_TURN, side);
        }
    }

    return 0;
}

int PlayerFighter::InputHandler_Attack(RegisteredInput& input){
    using Move = Champion::DefaultMoves;
    DirectionIG direction;
    Move move;
    if (!grounded){

        Debug::log(input.data);
        direction = (input.data & 1) ? Kuribrawl::DirectionToDirectionIG((Direction)(input.data >> 1), facing) : getControlDirection4IG();

        switch (direction){
            case DirectionIG::NONE:
                Debug::log("Nair");
                move = Move::Nair;
                break;
            case DirectionIG::UP:
                Debug::log("Uair");
                move = Move::UAir;
                break;
            case DirectionIG::DOWN:
                move = Move::Nair;
                Debug::log("Dair");
                break;
            case DirectionIG::FORWARD:
                move = Move::Nair;
                Debug::log("Fair");
                break;
            case DirectionIG::BACK:
                move = Move::Nair;
                Debug::log("Bair");
                break;
            default:
                break;
        }

        attack(*getChampion().getDefaultMove(move));
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