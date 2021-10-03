#include "ControllerType.h"
#include "Debug.h"
#include "Binding.h"

/**
 * @brief Construct a new Controller Type object.
 * 
 */

ControllerType::ControllerType():

    default_binding(std::make_unique<Binding>(this)),
    vals{10000, 18000, 20000}
{

}

/**
 * @brief Returns a structure containing all the fixed numerical values needed to read a controller of this type.
 * See ControllerType::ControllerVals.
 * @return const ControllerType::ControllerVals& vals.
 */

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