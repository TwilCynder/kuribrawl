#pragma once
#include <memory>
#include "Binding.h"
#include "util.h"
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


    //Needed ONLY when no SDL binding

    bool valid_element_mapping; ///< If false the content of element_mapping is uninitialized and unusable. 
    struct ControllerElementMapping {
        int start_button;       /**< Numerical id of the start button*/
        Kuribrawl::Vector control_stick       /**< UNUSED WHILE WE RELY ON SDL MAPPING. Numerical id of the control stick (the main stick)*/;
        Kuribrawl::Vector secondary_stick;    /**< UNUSED WHILE WE RELY ON SDL MAPPING*/
        ControllerType::DPadButtons direction_buttons;  /**< UNUSED WHILE WE RELY ON SDL MAPPING .AKA the dpad (will contain the SDL button IDs of the dpad)*/
    } element_mapping;


    std::unique_ptr<Binding> default_binding; ///< The Binding that will be used by default for controllers that are associated with this ControllerType
    ControllerVals vals; ///< The fixed numerical values needed to read this controller

    private:
    int nb_buttons; 
    int nb_sticks; //Sticks include triggers //no they don't
};