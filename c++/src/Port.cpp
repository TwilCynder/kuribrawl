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
    if (controller){
        current_dpad_state.x = controller->getDpadStateX();
        current_dpad_state.y = controller->getDpadStateY();
    }
}

//SDL GAMECONTROLLER
void Port::readController(){

    if (controller){
        controller->readController(elements_state);
    }

    if (pod.read_dpad){
        updateDpadState();
    }
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
    controller->clearPort();
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

const Controller *Port::getController() const
{
    return controller;
}

const ControllerType *Port::getControllerType() const
{
    return controller ? controller->getControllerType() : nullptr;
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

