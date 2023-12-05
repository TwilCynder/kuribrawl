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

#define MAPPING_NORMAL_SIZE 32
#define isKeyboard (joystick_id == JOSTICKID_KEYBOARD)

Port::Port(PortsManager& pm_, int id_) :
    ports_manager(pm_),
    id(id_),
    active(false),
    controller(nullptr),
    joystick_id(-1),
    fighter(nullptr),
    control_stick{0, 0},
    current_dpad_state{0, 0}
{
    Debug::out << "Constructed Port " << id << '\n';
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

void Port::handleJoystickButtonPress(Uint8 jbutton){
    if (jbutton >= controller_buttons_mapping.size()){
        return;
    }
    SDL_GameControllerButton cbutton = controller_buttons_mapping[jbutton];
    if (cbutton != SDL_CONTROLLER_BUTTON_INVALID){
        handleButtonPress(cbutton);
    }
}

inline void Port::handleButtonPress(Uint8 cbutton){
    if (!fighter) return;

    fighter->handleButtonPress(cbutton);
}

//SDL GAMECONTROLLER
bool Port::isJoystickButtonPressed(int button) const{
    if (isKeyboard) return SDL_GetKeyboardState(nullptr)[button];
	return SDL_JoystickGetButton(joystick, button);
}

bool Port::isButtonPressed(int button) const {
    if (isKeyboard) return SDL_GetKeyboardState(nullptr)[button];
	return SDL_GameControllerGetButton(controller, (SDL_GameControllerButton)button);
}

//SDL GAMECONTROLLER

/**
 * @brief DEPRECATED Checks if a trigger is pressed
 * @deprecated user the overload that takes a ControllerVals& now
 * @param trigger 
 */

bool Port::isTriggerPressed(int trigger) const {
    return isTriggerPressed(trigger, controller_type->getControllerVals());
}

/**
 * @brief Checks if a trigger is pressed given a certain threshold
 * 
 * @param trigger 
 * @param controller_vals 
 * @return whether the trigger is pressed (beyond the given threshold)
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

/**
 * @brief DEPRECATED checks if an element (button or trigger) is pressed
 * @deprecated user the overload that takes a ControllerVals& now
 * @param type 
 * @param element 
 * @return whether the element is pressed
 */
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

/**
 * @brief Checks if an element (button or trigger) is pressed, given certain ControllerVals
 * 
 * @param type type of element to check
 * @param element id of element to check
 * @param controller_vals controllerVals to base the answer on (most importantly, threshold for triggers)
 * @return whether the element is pressed
 */
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

//SDL GAMECONTROLLER
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
 * @brief Initializes controller_buttons_mapping with the current SDL Mapping for the current controller.  
 * Assumes the controller attribute is valid.
 * 
 * @return true 
 * @return false 
 */
bool Port::initButtonMapping(){
    size_t needed_size = MAPPING_NORMAL_SIZE;
    
    for (unsigned int i = 0; i < controller_buttons_mapping.size(); i++){
        controller_buttons_mapping[i] = SDL_CONTROLLER_BUTTON_INVALID;
    }


    Debug::log(SDL_GameControllerMapping(controller));
    for (unsigned int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++){
        SDL_GameControllerButtonBind bind = SDL_GameControllerGetBindForButton(controller, static_cast<SDL_GameControllerButton>(i));
        Debug::out << bind.bindType << " " << bind.value.button << " " << i << '\n';
        if (bind.bindType == SDL_CONTROLLER_BINDTYPE_NONE) continue;
        if (bind.bindType != SDL_CONTROLLER_BINDTYPE_BUTTON){
            return false;
        }
        unsigned int jbutton = bind.value.button;
        if (jbutton >= controller_buttons_mapping.size()){
            controller_buttons_mapping.resize(jbutton + 1, SDL_CONTROLLER_BUTTON_INVALID);
        }
        if (jbutton + 1 > needed_size) needed_size = jbutton + 1;

        controller_buttons_mapping[jbutton] = static_cast<SDL_GameControllerButton>(i);
    }

    if (controller_buttons_mapping.size() > needed_size){
        controller_buttons_mapping.resize(needed_size);
    }
    Debug::log(controller_buttons_mapping[3]);
    return true;
}

/**
 * @brief Binds this port to a controller (keyboard included)
 * Handles the creation of a SDL_GameController, and registers itself as the port for that joystick
 * @param id numerical ID given by the OS to the controller, or -1 for the keyboard
 * @returns false if a problem occured (in which case the port remains inactive), true otherwise
 */
bool Port::plugController_(int8_t controller_id){
    unregisterController();

    Debug::log("Plug Conroller");
    Debug::log(controller_id);

    if (controller_id == JOSTICKID_KEYBOARD){
        //REGISTERING CONTROLLER (see below)
        joystick_id = JOSTICKID_KEYBOARD;
        ports_manager.keyboard = this;
    } else {
        if (controller) SDL_GameControllerClose(controller);

        controller = SDL_GameControllerOpen(controller_id);
        joystick = SDL_GameControllerGetJoystick(controller);
        SDL_JoystickID instance_id = SDL_JoystickInstanceID(joystick);

        Debug::log("open controller");
        Debug::log(controller);
        Debug::log(joystick);

        if (instance_id < 0) {
            Debug::out << "Error while trying to plug controller " << controller_id << " into port " << id << " : ";
            Debug::log(SDL_GetError());
            controller = nullptr;
            joystick = nullptr;
            return false;
        }

        unregisterController();

        //REGISTERING CONTROLLER (pas mis dans une fonction à part car impossible de factoriser avec la procédure pour le clavier, flemme de faire 2 fonctions)
        /**
         * @todo remplacer le système de app->controllers[] par celui de player-index de SDL 
         */
        joystick_id = instance_id;
        ports_manager.controllers[instance_id] = this;
        
        //STORING THE BUTTON MAPPING
        if (!initButtonMapping()){
            Debug::out << "Error while trying to plug controller " << controller_id << " into port : the SDL mapping is ill-formed.\n";
            return false;
        }

        Debug::out << "Controller plugged ( " << controller_id << ") to port " << (id + 1) << " : " << SDL_GameControllerName(controller) << '\n' << std::flush;
    }

    Debug::log(this);
    active = true;
    return true;
}

/**
 * @brief Calls Port::plugController_, and determines the ControllerType object that will be used for this Port.
 * 
 * @param id the index of the controller
 * @param cd the ControllersData object that will be used to obtain a ControllerType
 */
void Port::plugController(int8_t id, ControllersData& cd){
    plugController_(id);

    if (isKeyboard){
        setControllerType(cd.getKeyboardController());
    } else {
        ControllerType* ct = cd.getControllerFromMapping(SDL_GameControllerMapping(controller));

        if (!ct){
            ct = cd.getDefaultController();
        }

        setControllerType(ct);
    }
}

void Port::setFighter(PlayerFighter* fighter_){
    fighter = fighter_;
    fighter->setPort(this);
    fighter->initPortOptimizationData(pod);
}

void Port::unregisterController(){
    if (active){
        if (joystick_id == JOSTICKID_KEYBOARD) ports_manager.keyboard = nullptr;
        else
            ports_manager.controllers[joystick_id] = nullptr;
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