#pragma once
#include <memory>
#include "inputs.h"

#define MAX_NB_BUTTONS 32
#define MAX_NB_STICKS 4

struct Binding {
    Input buttons[MAX_NB_BUTTONS];
    int tilt_stick;

};