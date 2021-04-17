#pragma once
#include <memory>
#include "Binding.h"

#define DEFAULT_NB_BUTTONS 24
#define DEFAULT_NB_STICKS 3

/**
 * @brief Type of controller.
 * Contains all information needed to manipulate a certain type of controller
 */

class ControllerType {
    public:

    /**
     * @brief Fixed numerical values needed to read a controller
     * 
     */
    struct ControllerVals {
        int analogStickThreshold; 
        int analogStickSmashThreshold;
        int analogTriggerThreshold;
    };

    ControllerType();

    const ControllerVals& getControllerVals() const;
    void setControllerVals(int ast, int att, int asst);

    int start_button; /**< Numerical id of the start button*/
    int control_stick /**< Numerical id of the control stick (the main stick)*/;

    std::unique_ptr<Binding> default_binding; ///< The Binding that will be used by default for controllers that are associated with this ControllerType

    private:
    ControllerVals vals; ///< The fixed numerical values needed to read this controller
    int nb_buttons; 
    int nb_sticks; //Sticks include triggers //no they don't
};