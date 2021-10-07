#pragma once
#include <memory>
#include "inputs.h"
#include "ControllerVals.h"

#define MAX_NB_BUTTONS 512//thank you SDL for these HUMONGOUS arrays
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
    Binding(ControllerType*, bool override_controller_vals = false);


    /**
     * @brief Array containing the input associated with each button (each button correspondig to an index of the array).
     * The concept of button here is a bit blurry, can be any controller event identified by a number, from GameController buttons
     * to keyboard keys. 
     */
    Input buttons[MAX_NB_BUTTONS]; 
    Input triggers[2];
    Input second_stick;

    ControllerVals controller_vals;
    bool override_controller_vals;

    DirectionControlMode direction_control_mode;
    bool tap_jump;
    int dpadAnalogValue;
    int dpadAnalogValueModified;
    int analog_modifier_button;

    ControllerType* controller; /**< The Controller this binding refers to. 1 if it's a keyboard binding. Pointer validity : can be invalidated if a controllerType is deleted (not supposed to happen) */
};