#pragma once 
#include "inputs.h"
#include "Port.h"

typedef struct {
    Input input;
    Port* port;
    int element;
    ElementType element_type;
    int data;
} RegisteredInput;