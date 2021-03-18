#include "Controller.h"
#include "Debug.h"

std::map<std::string, Controller> Controller::known_controllers;

Controller::Controller() : 
    default_binding(std::make_unique<Binding>())
{
    Debug::log(this);
    default_binding->controller = this;
    Controller(DEFAULT_NB_BUTTONS, DEFAULT_NB_STICKS);
}

Controller::Controller(int buttons, int sticks):
    nb_buttons(buttons),
    nb_sticks(sticks)
{
    Debug::log(this);
}

Controller* Controller::getController(const std::string& name){
    auto it = known_controllers.find(name);
    if (it == known_controllers.end()){
        throw KBFatal("Controller not found : " + name);
    } else {
        return &(it->second);
    }
    return nullptr;
}