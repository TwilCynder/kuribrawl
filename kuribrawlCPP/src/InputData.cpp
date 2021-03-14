#include "Controller.h"

void initControllersData(){
    auto [node, sucess] = Controller::known_controllers.emplace(std::make_pair("GC", Controller()));
    Controller& c = node->second;
    
    c.default_binding->buttons[0] = Input::ATTACK;
    c.default_binding->buttons[1] = Input::SPECIAL;
    c.default_binding->buttons[2] = Input::JUMP;
    c.default_binding->buttons[3] = Input::JUMP;
}