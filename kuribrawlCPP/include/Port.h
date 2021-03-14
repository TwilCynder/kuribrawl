#pragma once

#include <forward_list>
#include "Fighter.h"
#include "Controller.h"
#include "ControllerState.h"
#include "inputs.h"

#define CONTROL_STICK_FRAME_BUFFER 2 //TODO make it customizable ?

class Port {
    public:

    Port();
    ~Port();

    bool isActive();

    private:
    void swap_control_stick_buffers();

    Controller* controller; //null if inactive

    int id;
    int controller_id;
    bool active;

    Fighter* fighter;

    //Input input_pressed[Input::TOTAL];
    int* control_stick_buffer[2]; //contains pointers to 4-int arrays, each int representing a direction, each array represent a previous frame. Most recent first

    //ControllerState current_state;
    AxisState control_stick_state;
};