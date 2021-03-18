#pragma once

#include <forward_list>
#include "Fighter.h"
#include "Controller.h"
#include "Binding.h"
#include "ControllerState.h"
#include "inputs.h"

#define CONTROL_STICK_FRAME_BUFFER 2 //TODO make it customizable ?

class Port {
    public:
    static Port* joysticks[16];

    Port();
    ~Port();

    bool isActive() const ;
    void setJoystick(int id);
    void setJoystick(int id, Controller* controller);
    void deactivate();
    void setController(Controller* c);
    Controller* getController() const;
    Binding* getInputBinding() const;
    
    private:
    void swap_control_stick_buffers();
    void setJoystick_(int id);

    Controller* controller;
    Fighter* fighter;
    Binding* input_binding;

    int id;
    int joystick_id;
    SDL_Joystick* joystick;
    bool active;

    //Input input_pressed[Input::TOTAL];
    int* control_stick_buffer[2]; //contains pointers to 4-int arrays, each int representing a direction, each array represent a previous frame. Most recent first

    //ControllerState current_state;
    AxisState control_stick_state;
};