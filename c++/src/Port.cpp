#include "Port.h"
#include "InputManager.h"
#include "Debug.h"
#include "DebugInput.h"
#include "app.h"
#include "Binding.h"
#include <stdlib.h>

Port::Port(App* app_) : 
    app(app_),
    input_binding(0),
    joystick_id(-1),
    active(false)
{
}

bool Port::isActive() const{
    return active;
}

ControllerType* Port::getController() const {
    return controller;
}

void Port::setController(ControllerType* c){
    controller = c;
}

//use only with active ports : controller can be null if the port is inactive
Binding* Port::getInputBinding() const {
    return (input_binding && input_binding->controller == controller) ? input_binding : controller->default_binding.get();
}

void Port::handleButtonPress(int button){
    if (!fighter) return;

    Input input = getInputBinding()->buttons[button];
    InputManager* manager = fighter->getInputManager();
    manager->registerInput(input, this, button, ElementType::BUTTON, 0);
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

    control_stick.current_state.x = SDL_GameControllerGetAxis(joystick, SDL_CONTROLLER_AXIS_LEFTX);
    control_stick.current_state.y = SDL_GameControllerGetAxis(joystick, SDL_CONTROLLER_AXIS_LEFTY);
    secondary_stick.current_state.x = SDL_GameControllerGetAxis(joystick, SDL_CONTROLLER_AXIS_RIGHTX);
    secondary_stick.current_state.y = SDL_GameControllerGetAxis(joystick, SDL_CONTROLLER_AXIS_RIGHTY);

    //détection du passage de threshold
}

void Port::setJoystick_(int id){
    joystick = SDL_GameControllerOpen(id);
    SDL_JoystickID instance_id = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(joystick));

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
    if (!controller){
        throw KBFatal("Trying to enable port with no controller");
    }
    setJoystick_(id);
}

void Port::setFighter(PlayerFighter* fighter_){
    fighter = fighter_;
    fighter->setPort(this);
}

void Port::deactivate(){
    active = false;
}

void Port::Stick::updatePrevious(){
    previous_state.x = current_state.x;
    previous_state.y = current_state.y;
}