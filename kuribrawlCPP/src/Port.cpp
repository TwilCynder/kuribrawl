#include "Port.h"
#include "InputManager.h"
#include "Debug.h"
#include "DebugInput.h"
#include "app.h"
#include <stdlib.h>

Port::Port(App* app_) : 
    app(app_),
    joystick_id(-1),
    input_binding(0),
    debug(12),
    active(false)
{
    int* buffer = (int*)malloc(CONTROL_STICK_FRAME_BUFFER * 4);

    control_stick_buffer[0] = buffer;
    control_stick_buffer[1] = buffer + 4;
}

Port::~Port(){
    free(control_stick_buffer[0]);
}

void Port::swap_control_stick_buffers(){ 
    int* buffer = control_stick_buffer[1];
    control_stick_buffer[1] = control_stick_buffer[0];
    control_stick_buffer[0] = buffer;
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

const Kuribrawl::Vector& Port::getSecondaryStickState() const{
    return secondary_stick.current_state;
}

void Port::readController(){
    control_stick.current_state.x = SDL_GameControllerGetAxis(joystick, SDL_CONTROLLER_AXIS_LEFTX);
    control_stick.current_state.y = SDL_GameControllerGetAxis(joystick, SDL_CONTROLLER_AXIS_LEFTY);
    secondary_stick.current_state.x = SDL_GameControllerGetAxis(joystick, SDL_CONTROLLER_AXIS_RIGHTX);
    secondary_stick.current_state.y = SDL_GameControllerGetAxis(joystick, SDL_CONTROLLER_AXIS_RIGHTY);

    //détection du passage de threshold

    control_stick.updatePrevious();
    secondary_stick.updatePrevious();
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

void Port::setFighter(Fighter* fighter_){
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