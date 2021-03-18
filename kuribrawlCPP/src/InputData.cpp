#include "Controller.h"
#include "Debug.h"

Controller& Controller::addController(const std::string& name){
    auto [node, sucess] = Controller::known_controllers.try_emplace(name);
    if (!sucess){
        throw KBFatal("Could not add Controler " + name);
    } else {
        return node->second;
    }
}

void Controller::initControllersData(){
    Controller& c = Controller::addController("GC");

    c.control_stick = 0;
    c.analogStickThreshold = 300;
    c.analogStickSmashThreshold = 750;
    c.analogTriggerThreshold = -500;

    c.default_binding->buttons[0] = Input::ATTACK;
    c.default_binding->buttons[1] = Input::SPECIAL;
    c.default_binding->buttons[2] = Input::JUMP;
    c.default_binding->buttons[3] = Input::SHORTHOP;
    c.default_binding->tilt_stick = 1;
}