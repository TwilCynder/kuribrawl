#pragma once 
#include "inputs.h"

class Port;

struct RegisteredInput{
    Input input;
    Port* port;
    int element;
    ElementType element_type;
    int data;

    RegisteredInput(Input input, Port* port, int element, ElementType element_type, int data);
} ;