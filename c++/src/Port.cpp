#include "Port.h"
#include "InputManager.h"
#include "Debug.h"
#include "DebugInput.h"
#include "app.h"
#include "Binding.h"
#include <stdlib.h>

#include "controllerElements.h"

Port::Port(App* app_) :
    app(app_),
    joystick_id(-1),
    active(false)
{
}

bool Port::isActive() const{
    return active;
}

ControllerType* Port::getController() const {
    return controller_type;
}

void Port::setController(ControllerType* c){
    controller_type = c;
}

void Port::handleButtonPress(int button){
    if (!fighter) return;

    fighter->handleButtonPress(button);
}

bool Port::isButtonPressed(int button) const{
	return SDL_JoystickGetButton(joystick, button);
}

bool Port::isTriggerPressed(int trigger) const{
    int threshold = controller_type->getControllerVals().analogTriggerThreshold;
    switch(trigger){
        case TRIGGER_LEFT:
            return SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT) > threshold;
        case TRIGGER_RIGHT:
            return SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > threshold;
        default:
            return false;
    }
}

bool Port::isElementPressed(ElementType type, int element) const{
    switch(type){
        case ElementType::BUTTON:
            return isButtonPressed(element);
        case ElementType::TRIGGER:
            return isTriggerPressed(element);
        default:
            return false;
    }
}

const Kuribrawl::Vector& Port::getControlStickState() const{
    return control_stick.current_state;
}

const Kuribrawl::Vector& Port::getControlStickPreviousState() const{
    return control_stick.previous_state;
}

const Kuribrawl::Vector& Port::getSecondaryStickState() const{
    return secondary_stick.current_state;
}

void Port::readController(){

    control_stick.updatePrevious();
    secondary_stick.updatePrevious();
    left_trigger.updatePrevious();
    right_trigger.updatePrevious();

    control_stick.current_state.x = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
    control_stick.current_state.y = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
    secondary_stick.current_state.x = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);
    secondary_stick.current_state.y = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);

    left_trigger.current_state  = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT );
    right_trigger.current_state = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

    //détection du passage de threshold
    const ControllerType::ControllerVals& vals = controller_type->getControllerVals();

    if (pod.is_left_trigger_binding && left_trigger.current_state > vals.analogTriggerThreshold && left_trigger.previous_state < vals.analogTriggerThreshold){
        
        fighter->handleTriggerPress(TRIGGER_LEFT);
    }

    if (pod.is_right_trigger_binding && (right_trigger.current_state >= vals.analogTriggerThreshold) && (right_trigger.previous_state < vals.analogTriggerThreshold)){
        fighter->handleTriggerPress(TRIGGER_RIGHT);
    }
}

/**
 * @brief Returns the the horizontal state of the dpad
 * -1 for left, 0 for neutral, 1 for right
 * @return Uint8 
 */
signed char Port::getDpadStateX() const{
    return SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT) ? -1 : 
        (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) ? 1 : 0);
}

/**
 * @brief Returns the the vertical state of the dpad
 * -1 for up, 0 for neutral, 1 for down
 * @return Uint8 
 */
signed char Port::getDpadStateY() const{
    return SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN) ? 1 : 
        (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP) ? -1 : 0);
}

void Port::setJoystick_(int id){

    controller = SDL_GameControllerOpen(id);
	joystick = SDL_GameControllerGetJoystick(controller);
    SDL_JoystickID instance_id = SDL_JoystickInstanceID(joystick);

    Debug::log(SDL_GameControllerName(controller));

    if (active){
        app->joysticks[joystick_id] = nullptr;
    }

    if (instance_id < 0) {
        Debug::log(SDL_GetError());
        return;
    }

    active = true;
    joystick_id = instance_id;
    app->joysticks[instance_id] = this;
}

void Port::setJoystick(int id, ControllerType* c){
    if (!c){
        throw KBFatal("Trying to enable joystick with null controller pointer");
    }
    setController(c);
    setJoystick_(id);
}

void Port::setJoystick(int id){
    if (!controller_type){
        throw KBFatal("Trying to enable port with no controller");
    }
    setJoystick_(id);
}

void Port::setFighter(PlayerFighter* fighter_){
    fighter = fighter_;
    fighter->setPort(this);
    fighter->initPortOptimizationData(pod);
}

void Port::deactivate(){
    active = false;
}

inline void Port::Stick::updatePrevious(){
    previous_state.x = current_state.x;
    previous_state.y = current_state.y;
}

inline void Port::Trigger::updatePrevious(){
    previous_state = current_state;
}

/**
 * Note on the usual sequence of events when a PF is associated to a port
 * - Port::setFighter is called. It can be called "manually", or by the (Game*, int, int, Port&) constructor of PF
 * - PlayerFighter::setPort is called by Port::setFighter
 */