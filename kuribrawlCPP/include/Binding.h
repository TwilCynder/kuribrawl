#pragma once
#include <memory>
#include "inputs.h"

#define MAX_NB_BUTTONS 32
#define MAX_NB_STICKS 4

class ControllerType;

struct Binding {
    Input buttons[MAX_NB_BUTTONS];
    int tilt_stick;
    ControllerType* controller; //Pointer validity : can be invalidated if a controllerType is deleted (not supposed to happen)
};