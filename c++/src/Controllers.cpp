#include "Controllers.h"
#include "ControllersData.h"
#include "Port.h"

#define MAPPING_NORMAL_SIZE 32
#define isKeyboard (joystick_id == JOSTICKID_KEYBOARD)


bool Controller::init(int controller_id, ControllersData &cd)
{
    gamecontroller = SDL_GameControllerOpen(controller_id);
    joystick = SDL_GameControllerGetJoystick(gamecontroller);
    SDL_JoystickID instance_id = SDL_JoystickInstanceID(joystick);

    Debug::log("open controller");
    Debug::log(gamecontroller);
    Debug::log(joystick);

    if (instance_id < 0){
        Debug::out << "Error while trying to open controller " << controller_id << '\n';
        Debug::log(SDL_GetError());
        gamecontroller = nullptr;
        joystick = nullptr;
        return false;
    }

    joystick_id = instance_id;

    //STORING THE BUTTON MAPPING
    if (!initButtonMapping()){
        Debug::out << "Error while trying to open controller : the SDL mapping is ill-formed.\n";
        return false;
    }

    ControllerType* ct = cd.getControllerFromMapping(SDL_GameControllerMapping(gamecontroller));
    if (!ct) ct = cd.getDefaultController();
    setControllerType(ct);
}

unique_ptr<Controller> Controller::opencontroller(int controller_id, ControllersData &cd)
{
    unique_ptr<Controller> controller;
    Debug::out << "Constructing controller from ID " << controller_id << '\n';
    if (controller->init(controller_id, cd)) {
        return controller;
    } else {
        return nullptr;
    }
}

bool Controller::initAsKeyboard(ControllersData& cd){
    joystick_id = JOSTICKID_KEYBOARD;
    setControllerType(cd.getKeyboardController());

    gamecontroller = nullptr;
    joystick = nullptr;
}

unique_ptr<Controller> Controller::openKeyboardController(ControllersData& cd){
    unique_ptr<Controller> controller;
    Debug::out << "Constructing controller for keyboard\n";
    if (controller->initAsKeyboard(cd)) {
        return controller;
    } else {
        return nullptr;
    }
}

void Controller::plugToPort(Port & p)
{
    port = &p;
}

void Controller::unplug(){
    port = nullptr;
}



bool Controller::isButtonPressed(int id) const
{
    if (isKeyboard) return SDL_GetKeyboardState(nullptr)[id];
    return SDL_GameControllerGetButton(gamecontroller, (SDL_GameControllerButton)id);
}

bool Controller::isJoystickButtonPressed(int button) const
{
    if (isKeyboard) return SDL_GetKeyboardState(nullptr)[button];
	return SDL_JoystickGetButton(joystick, button);
}

/**
 * @brief DEPRECATED Checks if a trigger is pressed
 * @deprecated user the overload that takes a ControllerVals& now
 * @param trigger 
 */
bool Controller::isTriggerPressed(int trigger) const
{
    return isTriggerPressed(trigger, controller_type->getControllerVals());
}

/**
 * @brief Checks if a trigger is pressed given a certain threshold
 * 
 * @param trigger 
 * @param controller_vals 
 * @return whether the trigger is pressed (beyond the given threshold)
 */
bool Controller::isTriggerPressed(int trigger, const ControllerVals &controller_vals) const
{
    if (isKeyboard) return false;
    int threshold = controller_vals.analogTriggerThreshold;

    switch(trigger){
        case TRIGGER_LEFT:
            return SDL_GameControllerGetAxis(gamecontroller, SDL_CONTROLLER_AXIS_TRIGGERLEFT) > threshold;
        case TRIGGER_RIGHT:
            return SDL_GameControllerGetAxis(gamecontroller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > threshold;
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
bool Controller::isElementPressed(ElementType type, int element) const
{
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
bool Controller::isElementPressed(ElementType type, int element, const ControllerVals &controller_vals) const
{
    switch(type){
        case ElementType::BUTTON:
            return isButtonPressed(element);
        case ElementType::TRIGGER:
            return isTriggerPressed(element, controller_vals);
        default:
            return false;
    }
}

Sint16 getJoystickAxis(SDL_Joystick* joy, int axis){
    return (axis < 0) ? 0 : SDL_JoystickGetAxis(joy, axis);
}

void Controller::readController(Port::ElementsState& elements_state) const {
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
            elements_state.control_stick.current_state.x = SDL_GameControllerGetAxis(gamecontroller, SDL_CONTROLLER_AXIS_LEFTX);
            elements_state.control_stick.current_state.y = SDL_GameControllerGetAxis(gamecontroller, SDL_CONTROLLER_AXIS_LEFTY);
            elements_state.secondary_stick.current_state.x = SDL_GameControllerGetAxis(gamecontroller, SDL_CONTROLLER_AXIS_RIGHTX);
            elements_state.secondary_stick.current_state.y = SDL_GameControllerGetAxis(gamecontroller, SDL_CONTROLLER_AXIS_RIGHTY);
            
            elements_state.left_trigger.current_state  = SDL_GameControllerGetAxis(gamecontroller, SDL_CONTROLLER_AXIS_TRIGGERLEFT );
            elements_state.right_trigger.current_state = SDL_GameControllerGetAxis(gamecontroller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
        }

    }
}

void Controller::setControllerType(const ControllerType *c)
{
    controller_type = c;
    current_controller_layout = c->getElementLayout();
}

int Controller::getJoystickID() const {
    return joystick_id;
}

void Controller::handleButtonPress(Uint8 b)
{
    if (port){
        port->handleButtonPress(b);
    }
}

void Controller::handleJoystickButtonPress(Uint8 jb)
{
    if (jb >= controller_buttons_mapping.size()){
        return;
    }
    SDL_GameControllerButton cbutton = controller_buttons_mapping[jb];
    if (cbutton != SDL_CONTROLLER_BUTTON_INVALID){
        handleButtonPress(cbutton);
    }
}

/**
 * @brief Returns the the horizontal state of the dpad
 * -1 for left, 0 for neutral, 1 for right
 * @return Uint8 
 */
//SDL GAMECONTROLLER
inline signed char Controller::getDpadStateX() const
{
    if (isKeyboard){
        const Uint8* keyboard_state = SDL_GetKeyboardState(nullptr);

        return keyboard_state[current_controller_layout->direction_buttons.left] ? -1 : 
        (keyboard_state[current_controller_layout->direction_buttons.right] ? 1 : 0); 
    }
    return SDL_GameControllerGetButton(gamecontroller, SDL_CONTROLLER_BUTTON_DPAD_LEFT) ? -1 : 
        (SDL_GameControllerGetButton(gamecontroller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) ? 1 : 0);
}

/**
 * @brief Returns the the vertical state of the dpad
 * -1 for up, 0 for neutral, 1 for down
 * @return Uint8 
 */
//SDL GAMECONTROLLER
inline signed char Controller::getDpadStateY() const
{
    if (isKeyboard){
        const Uint8* keyboard_state = SDL_GetKeyboardState(nullptr);

        return keyboard_state[current_controller_layout->direction_buttons.up] ? -1 : 
        (keyboard_state[current_controller_layout->direction_buttons.down] ? 1 : 0); 
    }
    return SDL_GameControllerGetButton(gamecontroller, SDL_CONTROLLER_BUTTON_DPAD_DOWN) ? 1 : 
        (SDL_GameControllerGetButton(gamecontroller, SDL_CONTROLLER_BUTTON_DPAD_UP) ? -1 : 0);
}

/**
 * @brief Initializes controller_buttons_mapping with the current SDL Mapping for the controller.  
 * 
 * @return true 
 * @return false 
 */
bool Controller::initButtonMapping()
{
        size_t needed_size = MAPPING_NORMAL_SIZE;
    
    for (unsigned int i = 0; i < controller_buttons_mapping.size(); i++){
        controller_buttons_mapping[i] = SDL_CONTROLLER_BUTTON_INVALID;
    }


    Debug::log(SDL_GameControllerMapping(gamecontroller));
    for (unsigned int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++){
        SDL_GameControllerButtonBind bind = SDL_GameControllerGetBindForButton(gamecontroller, static_cast<SDL_GameControllerButton>(i));
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