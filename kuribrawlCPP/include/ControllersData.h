#pragma once
#include <map>
#include <string>
#include "ControllerType.h"

class ControllersData {
    public:
    std::map<std::string, ControllerType> known_controllers;
    ControllerType& addController(const std::string& name);
    ControllerType* getController(const std::string& name);
    void initControllersData();
};