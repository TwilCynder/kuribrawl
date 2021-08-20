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
    dpadAnalogValue(0),
    controller(ct)
{
    for (int i = 0; i < MAX_NB_BUTTONS; i++){
        buttons[i] = Input::NONE;
    }
    for (int i = 0; i < MAX_NB_TRIGGERS; i++){
        triggers[i] = Input::NONE;
    }
}