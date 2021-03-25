#pragma once
#include <memory>
#include "Binding.h"

#define DEFAULT_NB_BUTTONS 24
#define DEFAULT_NB_STICKS 3

class ControllerType {
    public:

    struct ControllerVals {
        int analogStickThreshold;
        int analogStickSmashThreshold;
        int analogTriggerThreshold;
    };

    ControllerType(int nb_buttons, int nb_sticks);
    ControllerType();

    const ControllerVals& getControllerVals() const;
    void setControllerVals(int ast, int att, int asst);

    int start_button;
    int control_stick;

    std::unique_ptr<Binding> default_binding;

    private:
    ControllerVals vals;
    int nb_buttons;
    int nb_sticks; //Sticks include triggers 
};