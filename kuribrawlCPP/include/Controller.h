#pragma once
#include <memory>
#include "inputs.h"

#define DEFAULT_NB_BUTTONS 24
#define DEFAULT_NB_STICKS 3

struct Binding {
    std::unique_ptr<Input[]> buttons;
    private:
    int nb_buttons;
    int nb_sticks;
}

class Controller {
    public:
    static Controller GC;

    Controller(int nb_buttons, int nb_sticks);
    Controller();

    private:
    int nb_buttons;
    int nb_sticks; //Sticks include triggers 
};