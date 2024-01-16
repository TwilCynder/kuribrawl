#include "Controllers.h"
#include "ControllersData.h"
#include "Port.h"

#define MAPPING_NORMAL_SIZE 32
#define isKeyboard (joystick_id == JOSTICKID_KEYBOARD)


bool Controller::init(int controller_id, ControllersData& cd){
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