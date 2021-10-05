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
    sdl_mode(Port::SDLMode::INVALID),
    controller(nullptr),
    joystick_id(-1),
    current_dpad_state({0, 0})
{
}

bool Port::isActive() const{
    return active;
}

ControllerType* Port::getController() const {
    return controller_type;
}

void Port::setControllerType(ControllerType* c){
    controller_type = c;
}

void Port::handleButtonPress(int button){
    if (!fighter) return;

    fighter->handleButtonPress(button);
}

///SDL GAMECONTROLLER
bool Port::isButtonPressed(int button) const{
    if (isKeyboard) return SDL_GetKeyboardState(nullptr)[button];
	return SDL_JoystickGetButton(joystick, button);
}

///SDL GAMECONTROLLER
bool Port::isTriggerPressed(int trigger) const{
    if (isKeyboard) return false;
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

///SDL GAMECONTROLLER
void Port::readController(){

    control_stick.updatePrevious();
    secondary_stick.updatePrevious();
    left_trigger.updatePrevious();
    right_trigger.updatePrevious();

    if (!isKeyboard){
        control_stick.current_state.x = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
        control_stick.current_state.y = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
        secondary_stick.current_state.x = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);
        secondary_stick.current_state.y = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);

        
        left_trigger.current_state  = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT );
        right_trigger.current_state = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
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
        return app->keyboard_state[SDL_SCANCODE_A] ? -1 : 
        (app->keyboard_state[SDL_SCANCODE_D] ? 1 : 0); 
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
        return app->keyboard_state[SDL_SCANCODE_W] ? -1 : 
        (app->keyboard_state[SDL_SCANCODE_S] ? 1 : 0); 
    }
    return SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN) ? 1 : 
        (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP) ? -1 : 0);
}

const Kuribrawl::VectorT<int8_t>& Port::getDpadState() const{
    return current_dpad_state;
}

/**
 * @brief Binds this port to a controller (keyboard included)
 * 
 * @param id numerical ID given by the OS to the controller, or -1 for the keyboard
 */
///SDL GAMECONTROLLER
void Port::setJoystick_(int id){

    if (id == JOSTICKID_KEYBOARD){
        app->keyboard = this;
    } else {

        if (controller)
            SDL_GameControllerClose(controller);

        controller = SDL_GameControllerOpen(id);
        joystick = SDL_GameControllerGetJoystick(controller);
        SDL_JoystickID instance_id = SDL_JoystickInstanceID(joystick);

        cout << "Controller plugged ( " << instance_id << ") : " << SDL_GameControllerName(controller) << '\n' << std::flush;

        if (instance_id < 0) {
            Debug::log(SDL_GetError());
            return;
        }

        unregisterController();
        joystick_id = instance_id;
        app->controllers[instance_id] = this;
    }
    active = true;
}

void Port::setJoystick(int id, ControllersData& cd){
    setJoystick_(id);

    if (isKeyboard){
        setControllerType(cd.getKeyboardController());
    } else {
        ControllerType* ct = cd.getControllerFromMapping(SDL_GameControllerMapping(controller));
    
        Debug::log(SDL_GameControllerMapping(controller));

        if (!ct)
            throw KBFatalDetailed("Tried to set joystick using controllerType detection but mapping matches no controllerType", "Unknown controller");

        setControllerType(ct);
    }
}

void Port::setJoystick(int id){
    if (!controller_type){
        throw KBFatal("Trying to enable port with no controller");
    }
    setJoystick_(id);
}

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