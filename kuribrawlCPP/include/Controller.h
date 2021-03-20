#pragma once
#include <memory>
#include "Binding.h"

#define DEFAULT_NB_BUTTONS 24
#define DEFAULT_NB_STICKS 3

class Controller {
    public:

    Controller(int nb_buttons, int nb_sticks);
    Controller();

    int analogStickThreshold;
    int analogStickSmashThreshold;
    int analogTriggerThreshold;
    int start_button;
    int control_stick;

    std::unique_ptr<Binding> default_binding;

    private:
    int nb_buttons;
    int nb_sticks; //Sticks include triggers 
};