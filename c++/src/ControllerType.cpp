#include "ControllerType.h"
#include "Debug.h"
#include "Binding.h"

ControllerType::ControllerType() : 
    ControllerType(DEFAULT_NB_BUTTONS, DEFAULT_NB_STICKS)
{
}

ControllerType::ControllerType(int buttons, int sticks):
    nb_buttons(buttons),
    nb_sticks(sticks),
    default_binding(std::make_unique<Binding>())
{
    default_binding->controller = this;
}

const ControllerType::ControllerVals& ControllerType::getControllerVals() const{
    return vals;
}


/**
 * Sets the values for this controller
 * @param AnalogStickThreshold
 * @param AnalogTriggerThreshold
 * @param AnalogSmashStickThreshold
 */
void ControllerType::setControllerVals(int ast, int att, int asst){
    vals.analogStickThreshold = ast;
    vals.analogTriggerThreshold = att;
    vals.analogStickSmashThreshold = asst;
}