#include "ControllersData.h"
#include "Debug.h"

Controller* ControllersData::getController(const std::string& name){
    auto it = known_controllers.find(name);
    if (it == known_controllers.end()){
        throw KBFatal("Controller not found : " + name);
    } else {
        return &(it->second);
    }
    return nullptr;
}

Controller& ControllersData::addController(const std::string& name){
    auto [node, sucess] = known_controllers.try_emplace(name);
    if (!sucess){
        throw KBFatal("Could not add Controler " + name);
    } else {
        return node->second;
    }
}