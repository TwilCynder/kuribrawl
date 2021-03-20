#include "Port.h"
#include "Debug.h"
#include "DebugInput.h"
#include <stdlib.h>

Port* Port::joysticks[16];

Port::Port() : 
    joystick_id(-1),
    input_binding(0)
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

Controller* Port::getController() const {
    return controller;
}

void Port::setController(Controller* c){
    controller = c;
}


//use only with active ports : controller can be null if the port is inactive
Binding* Port::getInputBinding() const {
    return (input_binding && input_binding->controller == controller) ? input_binding : controller->default_binding.get();
}

void Port::handleButtonPress(int button){
    Input input = getInputBinding()->buttons[button];
	Debug::log(input);
}

void Port::setJoystick_(int id){
    joystick = SDL_JoystickOpen(id);
    SDL_JoystickID instance_id = SDL_JoystickInstanceID(joystick);

    if (active){
        joysticks[joystick_id] = nullptr;
    }
    active = true;
    joystick_id = instance_id;
    joysticks[instance_id] = this;
}

void Port::setJoystick(int id, Controller* c){
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

void Port::deactivate(){
    active = false;
}