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

unique_ptr<Controller> Controller::opencontroller(int controller_id, ControllersData& cd)
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

bool Controller::isButtonPressed(int id) const
{
    if (isKeyboard) return SDL_GetKeyboardState(nullptr)[id];
    return SDL_GameControllerGetButton(gamecontroller, (SDL_GameControllerButton)id);
}

void Controller::setControllerType(const ControllerType *c)
{
    controller_type = c;
    current_controller_layout = c->getElementLayout();
}

int Controller::getJoystickID() const {
    return joystick_id;
}

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