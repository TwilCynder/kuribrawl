#include "ControllerType.h"
#include "KBDebug/Debug.h"
#include "Binding.h"

/**
 * @brief Construct a new Controller Type object.
 * 
 */

ControllerType::ControllerType():

    default_vals{10000, 18000, 20000},
    default_binding(std::make_unique<Binding>(this))
{

}

/**
 * @brief Returns a structure containing all the fixed numerical values needed to read a controller of this type.
 * See ControllerType::ControllerVals.
 * @return const ControllerType::ControllerVals& vals.
 */

const ControllerVals& ControllerType::getControllerVals() const{
    return default_vals;
}


/**
 * Sets the values for this controller
 * @param AnalogStickThreshold
 * @param AnalogTriggerThreshold
 * @param AnalogSmashStickThreshold
 */
void ControllerType::setControllerVals(int ast, int att, int asst){
    default_vals.analogStickThreshold = ast;
    default_vals.analogTriggerThreshold = att;
    default_vals.analogStickSmashThreshold = asst;
}

ControllerLayout* ControllerType::getElementLayout() const{
    return element_mapping.get();
}

ControllerLayout& ControllerType::addElementLayout(){
    element_mapping = std::make_unique<ControllerLayout>();
    return *element_mapping.get();
}

const Binding* ControllerType::getDefaultBinding(){
    return default_binding.get();
}

const Binding& ControllerType::getDefaultBinding() const{
    return *default_binding.get();
}