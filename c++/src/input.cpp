#include "input.h"
#include "GameConstantsBase.h"

/**
 * @brief Construct a new Registered Input object
 * See the member list for an explanation of the different parameters.
 * @param input_ 
 * @param port_ 
 * @param element_ 
 * @param element_type_ 
 * @param data_ 
 */
RegisteredInput::RegisteredInput(Input input_, int element_ = -1, ElementType element_type_, Kuribrawl::Direction direction_, int data_) : 
    input(input_),
    //port(port_),
    element(element_),
    element_type(element_type_),
    direction(direction_),
    data(data_),
    durability(game_constants.input_durability)
{
}