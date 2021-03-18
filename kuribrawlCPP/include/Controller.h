#pragma once
#include <memory>
#include <map>
#include <string>
#include "Binding.h"

#define DEFAULT_NB_BUTTONS 24
#define DEFAULT_NB_STICKS 3

class Controller {
    public:
    static std::map<std::string, Controller> known_controllers;
    static void initControllersData();
    static Controller& addController(const std::string& name);
    static Controller* getController(const std::string& name);

    Controller(int nb_buttons, int nb_sticks);
    Controller();

    std::unique_ptr<Binding> default_binding;

    private:
    int nb_buttons;
    int nb_sticks; //Sticks include triggers 
    int analogStickThreshold;
    int analogStickSmashThreshold;
    int analogTriggerThreshold;
    int start_button;
    int control_stick;
};