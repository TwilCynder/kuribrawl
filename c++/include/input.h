#pragma once 
#include "inputs.h"

class Port;

/**
 * @brief A player input.
 * Those are inputs as they registred by InputManagers.
 */
struct RegisteredInput{
    Input input;    ///< The game Input that was inputted.
    Port* port;     ///< The Port that made the Input.
    int element;    ///< The numerical ID of the controller element this input was done with.
    ElementType element_type;   ///<The type of the controller element that made the input.
    int data;       ///< Arbitrary data.

    RegisteredInput(Input input, Port* port, int element, ElementType element_type, int data);
} ;