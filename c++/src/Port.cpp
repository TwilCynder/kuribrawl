#include "Port.h"
#include "InputManager.h"
#include "KBDebug/Debug.h"
#include "KBDebug/DebugInput.h"
#include "app.h"
#include "Binding.h"
#include "ControllersData.h"
#include "controllerElements.h"
#include "PortsManager.h"
#include "defs.h"
#include "KBDebug/Debug.h"
#include "Controllers.h"

#define MAPPING_NORMAL_SIZE 32
#define isKeyboard (joystick_id == JOSTICKID_KEYBOARD)

Port::Port(PortsManager& pm_, int id_) :
    ports_manager(pm_),
    id(id_),
    controller(nullptr),
    fighter(nullptr),
    current_dpad_state{0, 0}
{
    Debug::out << "Constructed Port " << id << '\n';
}

bool Port::isActive() const{
    return !!controller;
}

inline void Port::handleButtonPress(Uint8 cbutton){
    if (!fighter) return;

    fighter->handleButtonPress(cbutton);
}

//SDL GAMECONTROLLER
bool Port::isJoystickButtonPressed(int button) const{
    return controller->isJoystickButtonPressed(button);
}

bool Port::isButtonPressed(int button) const {
    return controller->isButtonPressed(button);
}

//SDL GAMECONTROLLER

/**
 * @brief DEPRECATED Checks if a trigger is pressed
 * @deprecated user the overload that takes a ControllerVals& now
 * @param trigger 
 */

bool Port::isTriggerPressed(int trigger) const {
    return controller->isTriggerPressed(trigger);
}

/**
 * @brief Checks if a trigger is pressed given a certain threshold
 * 
 * @param trigger 
 * @param controller_vals 
 * @return whether the trigger is pressed (beyond the given threshold)
 */

bool Port::isTriggerPressed(int trigger, const ControllerVals& controller_vals) const{
    return controller->isTriggerPressed(trigger, controller_vals);
}

/**
 * @brief DEPRECATED checks if an element (button or trigger) is pressed
 * @deprecated user the overload that takes a ControllerVals& now
 * @param type 
 * @param element 
 * @return whether the element is pressed
 */
bool Port::isElementPressed(ElementType type, int element) const{
    return controller->isElementPressed(type, element);
}

/**
 * @brief Checks if an element (button or trigger) is pressed, given certain ControllerVals
 * 
 * @param type type of element to check
 * @param element id of element to check
 * @param controller_vals controllerVals to base the answer on (most importantly, threshold for triggers)
 * @return whether the element is pressed
 */
bool Port::isElementPressed(ElementType type, int element, const ControllerVals& controller_vals) const{
    return controller->isElementPressed(type, element, controller_vals);
}

const Port::StickState& Port::getControlStickState() const{
    return elements_state.control_stick;
}


const Port::StickState& Port::getSecondaryStickState() const{
    return elements_state.secondary_stick;
}

const Port::TriggerState& Port::getLeftTriggerState()  const {
    return elements_state.left_trigger;
}

const Port::TriggerState& Port::getRightTriggerState() const {
    return elements_state.right_trigger;
}

void Port::updateDpadState(){
    current_dpad_state.x = getDpadStateX();
    current_dpad_state.y = getDpadStateY();
}

Sint16 getJoystickAxis(SDL_Joystick* joy, int axis){
    return (axis < 0) ? 0 : SDL_JoystickGetAxis(joy, axis);
}

//SDL GAMECONTROLLER
void Port::readController(){

    elements_state.updatePrevious();

    if (!isKeyboard){
        if (current_controller_layout){
            elements_state.control_stick.current_state.x = getJoystickAxis(joystick, current_controller_layout->control_stick.x);
            elements_state.control_stick.current_state.y = getJoystickAxis(joystick, current_controller_layout->control_stick.y);
            elements_state.secondary_stick.current_state.x = getJoystickAxis(joystick, current_controller_layout->secondary_stick.x);
            elements_state.secondary_stick.current_state.y = getJoystickAxis(joystick, current_controller_layout->secondary_stick.y);

            elements_state.left_trigger.current_state  = getJoystickAxis(joystick, current_controller_layout->triggers.left);
            elements_state.right_trigger.current_state = getJoystickAxis(joystick, current_controller_layout->triggers.right);
        } else {    
            elements_state.control_stick.current_state.x = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
            elements_state.control_stick.current_state.y = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
            elements_state.secondary_stick.current_state.x = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);
            elements_state.secondary_stick.current_state.y = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);
            
            elements_state.left_trigger.current_state  = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT );
            elements_state.right_trigger.current_state = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
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
//SDL GAMECONTROLLER
signed char Port::getDpadStateX() const{
    if (isKeyboard){
        return ports_manager.keyboard_state[current_controller_layout->direction_buttons.left] ? -1 : 
        (ports_manager.keyboard_state[current_controller_layout->direction_buttons.right] ? 1 : 0); 
    }
    return SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT) ? -1 : 
        (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) ? 1 : 0);
}

/**
 * @brief Returns the the vertical state of the dpad
 * -1 for up, 0 for neutral, 1 for down
 * @return Uint8 
 */
//SDL GAMECONTROLLER
signed char Port::getDpadStateY() const{
    if (isKeyboard){
        return ports_manager.keyboard_state[current_controller_layout->direction_buttons.up] ? -1 : 
        (ports_manager.keyboard_state[current_controller_layout->direction_buttons.down] ? 1 : 0); 
    }
    return SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN) ? 1 : 
        (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP) ? -1 : 0);
}

const Port::DpadState& Port::getDpadState() const{
    return current_dpad_state;
}

inline double Port::normalizeStickValue(int value){
    return value / (double)ANALOG_MAX_VALUE;
}

Kuribrawl::Vec2<double> Port::normalizeStickVector(Kuribrawl::Vector& value){
    return {normalizeStickValue(value.x), normalizeStickValue(value.y)};
}


/**
 * @brief Binds this port to a controller (keyboard included)
 * @param id numerical ID given by the OS to the controller, or -1 for the keyboard
 * @returns false if a problem occured (in which case the port remains inactive), true otherwise
 */
void Port::plugController(Controller& cont, ControllersData& cd){
        cont.plugToPort(*this);
    controller = &cont;
}

void Port::unplugController()
{
    controller->unplug();
    controller = nullptr;
}

void Port::setFighter(PlayerFighter* fighter_){
    fighter = fighter_;
    fighter->setPort(this);
    fighter->initPortOptimizationData(pod);
}

void Port::deactivate(){
    unplugController();
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

Port::StickState::StickState() : current_state{0, 0}, previous_state{0, 0} {}

inline void Port::StickState::updatePrevious()
{
    previous_state.x = current_state.x;
    previous_state.y = current_state.y;
}

Port::TriggerState::TriggerState() : current_state{0}, previous_state{0} {}

inline void Port::TriggerState::updatePrevious(){
    previous_state = current_state;
}

void Port::ElementsState::updatePrevious()
{
    control_stick.updatePrevious();
    secondary_stick.updatePrevious();
    left_trigger.updatePrevious();
    right_trigger.updatePrevious();
}


/**
 * Note on the usual sequence of events when a PF is associated to a port
 * - Port::setFighter is called. It can be called "manually", or by the (Game*, int, int, Port&) constructor of PF
 * - PlayerFighter::setPort is called by Port::setFighter
 */

