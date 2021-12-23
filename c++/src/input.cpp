#include "input.h"

constexpr int INPUT_DURABILITY = 10;

/**
 * @brief Construct a new Registered Input object
 * See the member list for an explanation of the different parameters.
 * @param input_ 
 * @param port_ 
 * @param element_ 
 * @param element_type_ 
 * @param data_ 
 */
RegisteredInput::RegisteredInput(Input input_, Port* port_, int element_ = -1, ElementType element_type_ = ElementType::UNKNOWN, int data_ = 0) : 
    input(input_),
    port(port_),
    element(element_),
    element_type(element_type_),
    data(data_),
    durability(INPUT_DURABILITY)
{
}