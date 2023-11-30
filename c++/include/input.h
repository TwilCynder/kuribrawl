#pragma once 
#include "inputs.h"
#include "Util/util.h"

class Port;

/**
 * @brief A player input.
 * Those are inputs as they registred by InputManagers.
 */
struct RegisteredInput{
    Input input;    ///< The game Input that was made.
    //const Port* port;     ///< The Port that made the Input.
    int element;    ///< The numerical ID of the controller element this input was done with.
    ElementType element_type;   ///<The type of the controller element that made the input.
    Kuribrawl::Direction direction;
    int data;       ///< Arbitrary data.
    int durability;  ///< The numer of frames this input will remain in the queue for (if is not used)

    RegisteredInput(Input input, int element, ElementType element_type_ = ElementType::UNKNOWN, Kuribrawl::Direction direction_ = Kuribrawl::Direction::NONE, int data_ = 0);
} ;