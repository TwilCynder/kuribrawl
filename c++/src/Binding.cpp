#include "Binding.h"

/**
 * @brief Construct a new Binding:: Binding object
 * 
 */
Binding::Binding():
    Binding(nullptr)
{

}

/**
 * @brief Construct a new Binding:: Binding object
 * 
 */
Binding::Binding(ControllerType* ct):
    direction_control_mode(DirectionControlMode::STICK_ONLY),
    tap_jump(false),
    dpadAnalogValue(32000),
    dpadAnalogValueModified(10000),
    analog_modifier_button(-1),
    controller(ct)

{
    for (int i = 0; i < MAX_NB_BUTTONS; i++){
        buttons[i] = Input::NONE;
    }
    for (int i = 0; i < MAX_NB_TRIGGERS; i++){
        triggers[i] = Input::NONE;
    }
    second_stick = Input::NONE;
}