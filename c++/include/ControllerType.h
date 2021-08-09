#pragma once
#include <memory>
#include "Binding.h"
#include <SDL2/SDL_types.h>

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

    struct DPadButtons {
        Uint8 right;
        Uint8 up;
        Uint8 left;
        Uint8 down;
    };

    ControllerType();

    const ControllerVals& getControllerVals() const;
    void setControllerVals(int ast, int att, int asst);

    int start_button;       /**< Numerical id of the start button*/
    int control_stick       /**< UNUSED WHILE WE RELY ON SDL MAPPING. Numerical id of the control stick (the main stick)*/;
    int secondary_stick;    /**< UNUSED WHILE WE RELY ON SDL MAPPING*/
    DPadButtons direction_buttons;  /**< UNUSED WHILE WE RELY ON SDL MAPPING .AKA the dpad (will contain the SDL button IDs of the dpad)*/

    std::unique_ptr<Binding> default_binding; ///< The Binding that will be used by default for controllers that are associated with this ControllerType

    private:
    ControllerVals vals; ///< The fixed numerical values needed to read this controller
    int nb_buttons; 
    int nb_sticks; //Sticks include triggers //no they don't
};