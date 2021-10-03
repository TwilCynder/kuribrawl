#pragma once
#include <memory>
#include "inputs.h"

#define MAX_NB_BUTTONS 512//yes
#define MAX_NB_STICKS 4
#define MAX_NB_TRIGGERS 2

class ControllerType;

/**
 * @brief Mapping between the elements of a controller and their effects.
 * Associates an Input with each button.
 */

struct Binding {
    enum class DirectionControlMode {
        BOTH,
        STICK_ONLY,
        DPAD_ONLY
    };

    Binding();
    Binding(ControllerType*);

    Input buttons[MAX_NB_BUTTONS]; ///< Array containing the input associated with each button (each button correspondig to an index of the array).
    Input triggers[2];
    Input second_stick;

    DirectionControlMode direction_control_mode;
    bool tap_jump;
    int dpadAnalogValue;

    ControllerType* controller; /**< The Controller this binding refers to. 1 if it's a keyboard binding. Pointer validity : can be invalidated if a controllerType is deleted (not supposed to happen) */
};