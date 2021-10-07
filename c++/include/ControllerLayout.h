#pragma once
#include "util.h"
#include "SDL2/SDL_types.h"

/**
 * @brief represents the layout of a controller, i.e. numerical element id's mapped to the role of the element of the controller.
 * This is completely separate from any gameplay concept.
 * (*Formerly ControllerType::ControllerElementMapping*)
 */
struct ControllerLayout {
    struct DPadButtons {
        Uint8 right;
        Uint8 up;
        Uint8 left;
        Uint8 down;
    };

    int start_button;       /**< Numerical id of the start button*/
    Kuribrawl::Vector control_stick       /**< UNUSED WHILE WE RELY ON SDL MAPPING. Numerical id of the control stick (the main stick)*/;
    Kuribrawl::Vector secondary_stick;    /**< UNUSED WHILE WE RELY ON SDL MAPPING*/
    DPadButtons direction_buttons;  /**< UNUSED WHILE WE RELY ON SDL MAPPING .AKA the dpad (will contain the SDL button IDs of the dpad)*/
};