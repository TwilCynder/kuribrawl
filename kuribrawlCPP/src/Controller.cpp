#include "Controller.h"

std::map<std::string, Controller> Controller::known_controllers;

Controller::Controller(){
    Controller(DEFAULT_NB_BUTTONS, DEFAULT_NB_STICKS);
}

Controller::Controller(int buttons, int sticks):
    nb_buttons(buttons),
    nb_sticks(sticks)
{
    default_binding = std::make_unique<Binding>();
}