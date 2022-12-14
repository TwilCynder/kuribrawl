#pragma once
#include "Util/util.h"
#include "SDL2/SDL_types.h"

/**
 * @brief represents the layout of a controller, i.e. numerical element id's mapped to the role of the element of the controller.
 * This is completely separate from any gameplay concept.
 * (*Formerly ControllerType::ControllerElementMapping*)
 */
struct ControllerLayout {
    struct DPadButtons {
        int8_t right;
        int8_t up;
        int8_t left;
        int8_t down;
    };

    ControllerLayout():
        control_stick{-1, -1},
        secondary_stick{-1, -1},
        direction_buttons{-1, -1, -1, -1},
        triggers{-1, -1}
    {

    }

    int start_button;       /**< Numerical id of the start button*/
    Kuribrawl::Vector control_stick       /**< UNUSED WHILE WE RELY ON SDL MAPPING. Numerical id of the control stick (the main stick)*/;
    Kuribrawl::Vector secondary_stick;    /**< UNUSED WHILE WE RELY ON SDL MAPPING*/
    DPadButtons direction_buttons;  /**< UNUSED WHILE WE RELY ON SDL MAPPING .AKA the dpad (will contain the SDL button IDs of the dpad)*/
    Kuribrawl::bipair<int8_t> triggers;
};