#include "PlayerFighter.h"
#include "InputManager.h"
#include "Port.h"
#include "PortOptimizationData.h"
#include "KBDebug/Debug.h"
#include "macros.h"
#include "Champion.h"
#include "controllerElements.h"
#include "ControllerVals.h"

constexpr ControllerVals default_controller_vals = {10000, 18000, 20000};

using namespace Kuribrawl;

PlayerFighter::PlayerFighter(Game& game, Champion* model):
    Fighter(game, model),
    port(nullptr),
    valid_port(false),
    input_manager(this),
    control_stick_buffer(StickBuffer(7), StickBuffer(7)),
    current_direction_control_state{0, 0} 
{
    init_control_stick_buffer();
}


PlayerFighter::PlayerFighter(Game& game, Champion* model, int x, int y):
    Fighter(game, model, x, y),
    port(nullptr),
    valid_port(false),
    input_manager(this),
    control_stick_buffer(StickBuffer(7), StickBuffer(7)),   
    current_direction_control_state{0, 0}
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

void PlayerFighter::initPortOptimizationData(PortOptimizationData& pod) const {

    pod.read_left_trigger  = input_binding->triggers[TRIGGER_LEFT] != Input::NONE;
    pod.read_right_trigger = input_binding->triggers[TRIGGER_RIGHT] != Input::NONE;
    pod.read_dpad = input_binding->direction_control_mode != Binding::DirectionControlMode::STICK_ONLY;
}

/**
 * @brief Returns the Port controlling this Fighter.
 *
 * @return Port*
 */
const Port* PlayerFighter::getPort() const {
    return port;
}

/**
 * @brief Sets the Port controlling this Fighter.
 * Makes the port active.
 * @param port_ a pointer to a Port.
 */
void PlayerFighter::setPort(Port* port_){
    if (!port_) throw KBFatal("null pointer passed to PlayerFighter::setPort");
    if (!port_->isActive()) throw KBFatal("Tried to assign inactive port to PlayerFighter");

    valid_port = true;
    port = port_;
    
    //see below
    const ControllerType* controller_type = port->getControllerType();
    if (!controller_type) throw KBFatal("PlayerFighter::setPort : port has no controllerType");

    input_binding = &controller_type->getDefaultBinding();
    if (!input_binding) throw KBFatalDetailed("Unrecognized Controller", "PlayerFighter assigned to port, ended up with no input binding");


    ///\todo At some point we'lle have to make a decision regarding whether ports need to have a valid controller type (currently, yes)
    //current_controller_vals = ((!input_binding->override_controller_vals)/* && port->getControllerType()*/) ? //check is currently unnecessary since port->getControllerType() must be non-null anyways
    //    controller_type->getControllerVals() :
    //    input_binding->controller_vals;
    
}

/**
 * @brief Unsets the current port, indicating that this PlayerFighter no longer has a port.
 * Not sure if this will be ever used but it kinda made sense to make it idk 
 */
void PlayerFighter::unsetPort(){
    valid_port = false;
    port = nullptr;
}

void PlayerFighter::swap_control_stick_buffer(){
    /*int* buffer = control_stick_buffer[1];
    control_stick_buffer[1] = control_stick_buffer[0];
    control_stick_buffer[0] = buffer;*/
}

void PlayerFighter::init_control_stick_buffer(){
    //unused
}

void PlayerFighter::handleButtonPress(int button){
    Input input = input_binding->buttons[button];
    if (input != Input::NONE)
    input_manager.registerInput(input, button, ElementType::BUTTON, getControlDirection4(), 0);
}

void PlayerFighter::handleTriggerPress(int trigger){
    Input input = input_binding->triggers[trigger];
    if (input != Input::NONE)
    input_manager.registerInput(input, trigger, ElementType::TRIGGER, 0);
}

void PlayerFighter::handleStickFlick(Direction direction){
    if (input_binding->second_stick != Input::NONE){
        input_manager.registerInput(input_binding->second_stick, 0, ElementType::STICK, direction);
    }
}

void PlayerFighter::update_control_stick_buffer(const Vector& current_state, const Vector& previous_state){
    const ControllerVals current_controller_vals = getCurrentControllerVals();

    //At this point we assume the buffer is actually containing the position of the stick 2 frames ago
    if (current_state.x > current_controller_vals.analogStickSmashThreshold
        && previous_state.x < current_controller_vals.analogStickSmashThreshold
        && control_stick_buffer.x.containsNot(1))
    {
        //Smash input right
		Debug::log("Smash Input Right");
        input_manager.registerInput(Input::RIGHT, -1, ElementType::STICK);
    } else if (current_state.x < -current_controller_vals.analogStickSmashThreshold
        && previous_state.x > -current_controller_vals.analogStickSmashThreshold
        && control_stick_buffer.x.containsNot(-1))
    {
        //Smash input left
		Debug::log("Smash Input Left");
        input_manager.registerInput(Input::LEFT, -1, ElementType::STICK);
    }
    
    if (current_state.y > current_controller_vals.analogStickSmashThreshold
        && previous_state.y < current_controller_vals.analogStickSmashThreshold
        && control_stick_buffer.y.containsNot(1))
    {
        //Smash input right
		Debug::log("Smash Input Down");
        input_manager.registerInput(Input::DOWN, -1, ElementType::STICK);
    } else if (current_state.y < -current_controller_vals.analogStickSmashThreshold
        && previous_state.y > -current_controller_vals.analogStickSmashThreshold
        && control_stick_buffer.y.containsNot(-1))
    {
        //Smash input left
		Debug::log("Smash Input UP");
        input_manager.registerInput(Input::UP, -1, ElementType::STICK);
    }

    //std::cout << control_stick_buffer.y << '\n' << std::flush;      

    if (current_state.x > current_controller_vals.analogStickThreshold){
        control_stick_buffer.x.add(1);
    } else if (current_state.x < -current_controller_vals.analogStickThreshold) {
        control_stick_buffer.x.add(-1);
    } else {
        control_stick_buffer.x.add(0);
    }

    if (current_state.y > current_controller_vals.analogStickThreshold){
        control_stick_buffer.y.add(1);
    } else if (current_state.y < -current_controller_vals.analogStickThreshold) {
        control_stick_buffer.y.add(-1);
    } else {
        control_stick_buffer.y.add(0);
    }
}

/**
 * @brief Updates the state of the direction control of the controller
 * Which is, depending on the binding settings, either the direction given by the control stick or the dpad.
 */
void PlayerFighter::updateDirectionControlState(){
    int value = (input_binding->analog_modifier_button != -1 && port->isButtonPressed(input_binding->analog_modifier_button)) ?
        input_binding->dpadAnalogValueModified :
        input_binding->dpadAnalogValue;

    const Port::DpadState& dpad_state = port->getDpadState();
    if (input_binding->direction_control_mode == Binding::DirectionControlMode::DPAD_ONLY){
        current_direction_control_state.x = dpad_state.x * value;
        current_direction_control_state.y = dpad_state.y * value;
    } else {
        current_direction_control_state = port->getControlStickState().current_state;

        const ControllerVals current_controller_vals = getCurrentControllerVals();
        if (
            input_binding->direction_control_mode == Binding::DirectionControlMode::BOTH &&
            (abs(current_direction_control_state.x) < current_controller_vals.analogStickThreshold &&abs(current_direction_control_state.y) < current_controller_vals.analogStickThreshold)
        )
        {
            current_direction_control_state.x = dpad_state.x * value;
            current_direction_control_state.y = dpad_state.y * value;
        }
    }
}

Direction PlayerFighter::getDirection4(const Kuribrawl::Vector& stick_state) const{
    return Kuribrawl::getDirection4(stick_state, getCurrentControllerVals().analogStickThreshold);
}

DirectionIG PlayerFighter::getDirection4IG(const Kuribrawl::Vector& stick_state) const{
    return Kuribrawl::getDirection4IG(stick_state, getCurrentControllerVals().analogStickThreshold, facing);
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

    const ControllerVals current_controller_vals = getCurrentControllerVals();

    if (!grounded && isDown(current_direction_control_state, current_controller_vals.analogStickSmashThreshold)){
        ground_interaction = GroundInteraction::SOFT;
    }

    switch (state){
        case State::IDLE:
            if (abs(current_direction_control_state.x) > current_controller_vals.analogStickThreshold){
                if (grounded){
                    setState(State::WALK, sign(current_direction_control_state.x));
                } else {
					applyAirAccel(sign(current_direction_control_state.x));
				}
            }
            break;
        case State::ATTACK:
            if (abs(current_direction_control_state.x) > current_controller_vals.analogStickThreshold){
                if (!grounded){
					applyAirAccel(sign(current_direction_control_state.x));
				}
            }
            break;
        case State::WALK:
            if (abs(current_direction_control_state.x) < current_controller_vals.analogStickThreshold){
                setState(State::IDLE);
            } else if (sign(current_direction_control_state.x) != facing){

                setState(State::WALK, -facing, 0, false);
            }
            break;
        case State::DASH:
            if (current_direction_control_state.x * facing < current_controller_vals.analogStickThreshold){
                setState(State::DASH_STOP);
            }
            break;
        case State::DASH_START:
            if (abs(current_direction_control_state.x) < current_controller_vals.analogStickThreshold){
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
    
    {
    const Kuribrawl::Vector previous_direction_control_state = current_direction_control_state;
    updateDirectionControlState();
    update_control_stick_buffer(current_direction_control_state, previous_direction_control_state);
    }

    const ControllerVals current_controller_vals = getCurrentControllerVals();

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
    input_manager.updateInputs();
}

/**
 * @brief returns the InputManager used by this Fighter.
 * @return InputManager*
 */
const InputManager* PlayerFighter::getInputManager() const{
    return &input_manager;
}

const ControllerVals &PlayerFighter::getCurrentControllerVals() const
{
    const ControllerType* controller_type = port->getControllerType();

    return 
        input_binding->override_controller_vals ? input_binding->controller_vals : (
        controller_type ? controller_type->getControllerVals() :
        default_controller_vals
        )
    ;

    ///\todo At some point we'lle have to make a decision regarding whether ports need to have a valid controller type (currently, yes)

}

/**
 * @brief returns the input binding this PlayerFighter is currently using
 * 
 */

Binding* PlayerFighter::getInputBinding()const{
    return input_binding;
}

/**
 * @brief Returns the type of jump that should be performed if a grounded jump occured right now.
 * Considers that the fighter is in the Jumpsquat state, so state_info is the id of the element that caused the jump.
 * @return jumpY
 */
jumpY PlayerFighter::decideGroundedJumpYType() const {

    const ControllerVals current_controller_vals = getCurrentControllerVals();

    if (((state_info >> 2) & 1) == 1) return jumpY::Short;

    ElementType elem_type = (ElementType)((state_info >> 3) & 0b11);
    int element = state_info >> 5;
    if (elem_type == ElementType::STICK && element == -1){ //This was a smash input (with tap jump enabled)
        return (current_direction_control_state.y < -current_controller_vals.analogStickThreshold) ? jumpY::Full : jumpY::Short ;
    } else {
        return port->isElementPressed(elem_type, element, current_controller_vals) ? jumpY::Full : jumpY::Short;
    }
}

/**
 * @brief Returns the type of jump that should be performed if a grounded jump occured right now.
 * @return jumpX
 */
jumpX PlayerFighter::decideJumpXType() const {
    int8_t orientation = sign(current_direction_control_state.x);
    return (abs(current_direction_control_state.x) > getCurrentControllerVals().analogStickThreshold) ?
        (orientation == facing) ? jumpX::Forward : jumpX::Backwards :
        jumpX::Normal;
}

int PlayerFighter::handleInput(RegisteredInput& input){
    InputHandler handler = input_handlers[input.input];
    return (handler) ? (this->*handler)(input) : 0;
}

/**
 * @brief Called by the input handlers of jump inputs  
 * (Input::JUMP and Input::SHORTHOP)
 * @param input 
 * @param type 
 * @return int 
 */
int PlayerFighter::jump_manager(RegisteredInput& input, jumpY type){
    if (grounded){
        if (state == Fighter::State::JUMPSQUAT && input.element_type != ElementType::STICK){ //if we pressed a second jump button/trigger basically
            state_info |= 0b100;  //set the jump Y type to short hop
        } else {
            setState(Fighter::State::JUMPSQUAT, 0, 0 addBitValue((Uint8)(type == jumpY::Short) ? 1 : 0, 2) addBitValue(input.element_type, 3) addBitValue(input.element, 5));
        }
    } else {
        return air_jump();
    }

    return 0;
}

//DEBUG

/**
 * @brief Displays visual debug info.  
 * @param displayArea : an indicator of the area that should be used ; if this method uses a different area (position or size) it must update displayArea and retur true.
 * w and h fields can be -1 : in this case drawDebugInfo MUST set them (and return true)
 * @return true if displayArea was changed
 * @return false if this method only drew on the area indicated by displayArea.
 */
bool PlayerFighter::drawDebugInfo(SDL_Renderer* target, SDL_Rect& displayArea){
    const int radius = Kuribrawl::min(displayArea.w, displayArea.h) * 0.4;
    Vector center = {displayArea.x + displayArea.w / 2, displayArea.y + displayArea.h / 2 };

    if (valid_port){
        SDL_SetRenderDrawColor(target, 0, 0, 255, 255);

        SDL_Rect box = {
            center.x - radius,
            center.y - radius,
            radius * 2,
            radius * 2
        };

        //can you believe there is no function in sdl to render a fucking circle
        //damn i forgot about that and i still can't believe it
        //forgot about all that again, there's no function to render a circle ???
        SDL_RenderDrawRect(target, &box);

        const ControllerVals current_controller_vals = getCurrentControllerVals();

        double analogThreshold = Port::normalizeStickValue(current_controller_vals.analogStickThreshold);
        double analogSmashThreshold = Port::normalizeStickValue(current_controller_vals.analogStickSmashThreshold);

        box = {
            (int)(center.x - analogThreshold * radius),
            (int)(center.y - analogThreshold * radius),
            (int)(analogThreshold * radius * 2),
            (int)(analogThreshold * radius * 2)
        };
        SDL_SetRenderDrawColor(target, 255, 0, 255, 150);
        SDL_RenderDrawRect(target, &box);
        box = {
            (int)(center.x - analogSmashThreshold * radius),
            (int)(center.y - analogSmashThreshold * radius),
            (int)(analogSmashThreshold * radius * 2),
            (int)(analogSmashThreshold * radius * 2)
        };
        SDL_SetRenderDrawColor(target, 255, 0, 0, 150);
        SDL_RenderDrawRect(target, &box);

        Vec2<double> pos = Port::normalizeStickVector(current_direction_control_state);
        SDL_SetRenderDrawColor(target, 0, 255, 0, 255);
        SDL_RenderDrawLine(target, center.x, center.y, center.x + pos.x * radius, center.y + pos.y * radius);
    } else {
         SDL_Rect box = {
            center.x - radius,
            center.y - radius,
            radius * 2,
            radius * 2
        };
        SDL_SetRenderDrawColor(target, 127, 127, 127, 255);
        SDL_RenderDrawRect(target, &box);
    }

    return false;
}