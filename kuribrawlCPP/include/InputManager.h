#pragma once

#include <deque>
#include "input.h"
#include "Port.h"

class InputManager {
    public:

    InputManager();

    void registerInput(Input input, Port* port, int element, ElementType element_type, int data);

    private:

    std::deque<RegisteredInput> inputQ;
};