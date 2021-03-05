#include "Controller.h"

Controller::Controller(){
    Controller(DEFAULT_NB_BUTTONS, DEFAULT_NB_STICKS);
}

Controller::Controller(int buttons, int sticks):
    nb_buttons(buttons),
    nb_sticks(sticks)
{
    
}