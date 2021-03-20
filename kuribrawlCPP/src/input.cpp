#include "input.h"

RegisteredInput::RegisteredInput(Input input_, Port* port_, int element_ = -1, ElementType element_type_ = ElementType::UNKNOWN, int data_ = 0) : 
    input(input_),
    port(port_),
    element(element_),
    element_type(element_type_),
    data(data_)
{}