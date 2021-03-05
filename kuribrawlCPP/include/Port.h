#pragma once

#include "Controller.h"

class Port {
    public:

    Port();
    bool isActive();

    private:
    Controller* controller; //null if inactive
}