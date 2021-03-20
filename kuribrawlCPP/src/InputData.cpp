#include "ControllersData.h"
#include "inputs.h"
#include "Debug.h"

void ControllersData::initControllersData(){
    Controller& c = addController("GC");

    c.control_stick = 0;
    c.analogStickThreshold = 300;
    c.analogStickSmashThreshold = 750;
    c.analogTriggerThreshold = -500;

    c.default_binding->buttons[0] = Input::ATTACK;
    c.default_binding->buttons[1] = Input::SPECIAL;
    c.default_binding->buttons[2] = Input::SHORTHOP;
    c.default_binding->buttons[3] = Input::JUMP;
    c.default_binding->tilt_stick = 1;
}