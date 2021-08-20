#pragma once
#include <memory>
#include "inputs.h"

#define MAX_NB_BUTTONS 32
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

    DirectionControlMode direction_control_mode;
    int dpadAnalogValue;

    ControllerType* controller; /**< The Controller this binding refers to. Pointer validity : can be invalidated if a controllerType is deleted (not supposed to happen) */
};