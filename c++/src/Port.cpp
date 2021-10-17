#include "Port.h"
#include "InputManager.h"
#include "Debug.h"
#include "DebugInput.h"
#include "app.h"
#include "Binding.h"
#include "ControllersData.h"
#include <stdlib.h>
#include "controllerElements.h"

#define isKeyboard (joystick_id == JOSTICKID_KEYBOARD)

Port::Port(App* app_) :
    app(app_),
    active(false),
    controller(nullptr),
    joystick_id(-1),
    current_dpad_state({0, 0})
{
}

bool Port::isActive() const{
    return active;
}

const ControllerType* Port::getControllerType() const {
    return controller_type;
}

void Port::setControllerType(const ControllerType* c){
    controller_type = c;
    current_controller_layout = c->getElementLayout();
}

void Port::handleButtonPress(int button){
    if (!fighter) return;

    fighter->handleButtonPress(button);
}

///SDL GAMECONTROLLER
bool Port::isJoystickButtonPressed(int button) const{
    if (isKeyboard) return SDL_GetKeyboardState(nullptr)[button];
	return SDL_JoystickGetButton(joystick, button);
}

bool Port::isButtonPressed(int button) const {
    if (isKeyboard) return SDL_GetKeyboardState(nullptr)[button];
	return SDL_JoystickGetButton(joystick, button);
}

///SDL GAMECONTROLLER

/**
 * @brief DEPRECATED Checks if a trigger is pressed
 * 
 * @param trigger 
 * @return true 
 * @return false 
 */

bool Port::isTriggerPressed(int trigger) const {
    return isTriggerPressed(trigger, controller_type->getControllerVals());
}

/**
 * @brief Checks if a trigger is pressed given a certain threshold
 * 
 * @param trigger 
 * @param controller_vals 
 * @return true 
 * @return false 
 */

bool Port::isTriggerPressed(int trigger, const ControllerVals& controller_vals) const{
    if (isKeyboard) return false;
    int threshold = controller_vals.analogTriggerThreshold;

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

bool Port::isElementPressed(ElementType type, int element, const ControllerVals& controller_vals) const{
    switch(type){
        case ElementType::BUTTON:
            return isButtonPressed(element);
        case ElementType::TRIGGER:
            return isTriggerPressed(element, controller_vals);
        default:
            return false;
    }
}

const Port::StickState& Port::getControlStickState() const{
    return control_stick;
}


const Port::StickState& Port::getSecondaryStickState() const{
    return secondary_stick;
}

const Port::TriggerState& Port::getLeftTriggerState()  const {
    return left_trigger;
}

const Port::TriggerState& Port::getRightTriggerState() const {
    return right_trigger;
}

void Port::updateDpadState(){
    current_dpad_state.x = getDpadStateX();
    current_dpad_state.y = getDpadStateY();
}

Sint16 getJoystickAxis(SDL_Joystick* joy, int axis){
    return (axis < 0) ? 0 : SDL_JoystickGetAxis(joy, axis);
}

///SDL GAMECONTROLLER
void Port::readController(){

    control_stick.updatePrevious();
    secondary_stick.updatePrevious();
    left_trigger.updatePrevious();
    right_trigger.updatePrevious();

    if (!isKeyboard){
        if (current_controller_layout){
            control_stick.current_state.x = getJoystickAxis(joystick, current_controller_layout->control_stick.x);
            control_stick.current_state.y = getJoystickAxis(joystick, current_controller_layout->control_stick.y);
            secondary_stick.current_state.x = getJoystickAxis(joystick, current_controller_layout->secondary_stick.x);
            secondary_stick.current_state.y = getJoystickAxis(joystick, current_controller_layout->secondary_stick.y);

            left_trigger.current_state  = getJoystickAxis(joystick, current_controller_layout->triggers.left);
            right_trigger.current_state = getJoystickAxis(joystick, current_controller_layout->triggers.right);
        } else {
            control_stick.current_state.x = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
            control_stick.current_state.y = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
            secondary_stick.current_state.x = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);
            secondary_stick.current_state.y = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);

            
            left_trigger.current_state  = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT );
            right_trigger.current_state = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
        }

    }

    if (pod.read_dpad){
        updateDpadState();
    }
}

/**
 * @brief Returns the the horizontal state of the dpad
 * -1 for left, 0 for neutral, 1 for right
 * @return Uint8 
 */
///SDL GAMECONTROLLER
signed char Port::getDpadStateX() const{
    if (isKeyboard){
        return app->keyboard_state[current_controller_layout->direction_buttons.left] ? -1 : 
        (app->keyboard_state[current_controller_layout->direction_buttons.right] ? 1 : 0); 
    }
    return SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT) ? -1 : 
        (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) ? 1 : 0);
}

/**
 * @brief Returns the the vertical state of the dpad
 * -1 for up, 0 for neutral, 1 for down
 * @return Uint8 
 */
///SDL GAMECONTROLLER
signed char Port::getDpadStateY() const{
    if (isKeyboard){
        return app->keyboard_state[current_controller_layout->direction_buttons.up] ? -1 : 
        (app->keyboard_state[current_controller_layout->direction_buttons.down] ? 1 : 0); 
    }
    return SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN) ? 1 : 
        (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP) ? -1 : 0);
}

const Kuribrawl::VectorT<int8_t>& Port::getDpadState() const{
    return current_dpad_state;
}

/**
 * @brief Binds this port to a controller (keyboard included)
 * Handles the creation of a SDL_GameController, and registers itself as the port for that joystick
 * @param id numerical ID given by the OS to the controller, or -1 for the keyboard
 */
///SDL GAMECONTROLLER
void Port::plugController_(int controller_id){
    unregisterController();

    if (controller_id == JOSTICKID_KEYBOARD){
        joystick_id = JOSTICKID_KEYBOARD;
        app->keyboard = this;
    } else {
        if (controller)
            SDL_GameControllerClose(controller);

        controller = SDL_GameControllerOpen(controller_id);
        joystick = SDL_GameControllerGetJoystick(controller);
        SDL_JoystickID instance_id = SDL_JoystickInstanceID(joystick);

        if (instance_id < 0) {
            Debug::log(SDL_GetError());
            return;
        }

        cout << "Controller plugged ( " << instance_id << ") to port " << id << " : " << SDL_GameControllerName(controller) << '\n' << std::flush;

        unregisterController();
        joystick_id = instance_id;
        app->controllers[instance_id] = this;
    }
    active = true;
}

/**
 * @brief Calls Port::plugController_, and determines the ControllerType object that will be used for this Port.
 * 
 * @param id the index of the controller
 * @param cd the ControllersData object that will be used to obtain a ControllerType
 */
void Port::plugController(int id, ControllersData& cd){
    plugController_(id);

    if (isKeyboard){
        setControllerType(cd.getKeyboardController());
    } else {
        ControllerType* ct = cd.getControllerFromMapping(SDL_GameControllerMapping(controller));

        if (!ct){
            ct = cd.getDefaultController();
        }

        controller_type = ct;
    }

    current_controller_layout = controller_type->getElementLayout();
}

void Port::setFighter(PlayerFighter* fighter_){
    fighter = fighter_;
    fighter->setPort(this);
    fighter->initPortOptimizationData(pod);
}

void Port::unregisterController(){
    if (active){
        if (joystick_id == JOSTICKID_KEYBOARD) app->keyboard = nullptr;
        else
            app->controllers[joystick_id] = nullptr;
    }
}

void Port::deactivate(){
    unregisterController();
    active = false;
}
/*
void closeSDLInstance(){
    if (controller){
        SDL_GameControllerClose(controller);
    } else if (joystick){
        SDL_JoystickClose(joystick);
    }
}

void openJoystick(int id){
    closeSDLInstance();

    controller = nullptr;
    joystick = SDL_JoystickOpen(id);

    if (!joystick){
        throw KBFatalExplicit("Failed to open joystick");
    }

}

bool openGameController(int id){
    closeSDLInstance()

    controller = SDL_GameControllerOpen(int id);
    if (!controller) return false;
    joystick = SDL_GameControllerGetJoystick(controller);
}

//Assumes a controller was just opened

void setController_(ControllerType* controller_type_){
    controller_type = controller_type_;
    unregisterController();
    SDL_JoystickID instance_id = SDL_JoystickInstanceID(joystick);
    joystick_id = instance_id;
    app->controllers[instance_id] = this;
    active = true;
}

void setController_Joystick(int id, ControllerType* controller_type){
    openJoystick(id);
    setController_();
}

void setController_GameController(int id, ControllerType* controller){
    openGameController(id);
    setController_();
}

*/

inline void Port::StickState::updatePrevious(){
    previous_state.x = current_state.x;
    previous_state.y = current_state.y;
}

inline void Port::TriggerState::updatePrevious(){
    previous_state = current_state;
}

/**
 * Note on the usual sequence of events when a PF is associated to a port
 * - Port::setFighter is called. It can be called "manually", or by the (Game*, int, int, Port&) constructor of PF
 * - PlayerFighter::setPort is called by Port::setFighter
 */