#pragma once
#include <map>
#include <string>
#include "Controller.h"

class ControllersData {
    public:
    std::map<std::string, Controller> known_controllers;
    Controller& addController(const std::string& name);
    Controller* getController(const std::string& name);
    void initControllersData();
};