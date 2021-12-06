#pragma once
#include <map>
#include <string>
#include <memory>
#include "ControllerType.h"

#define MAPPING_ID_LENGTH 24

/**
 * @brief Class containing all data related to controllers.
 * Contains every supported ControllerType, associated with a string identifier.
 */

class ControllersData {
    using ControllersMap = std::map<std::string, ControllerType>;
    using ControllerMapKVP = ControllersMap::value_type;

    public:
    ControllersData();

    ControllerType& addController(const std::string& name);
    ControllerType* getController(const std::string& name);
    void mapSDLMappingToControllerType(const char* mapping_string, ControllerType* ct);
    ControllerType* getControllerFromMapping(const char* mapping_string) const;
    const ControllerType* getKeyboardController() const;
    ControllerType* getKeyboardController();
    const ControllerType* getDefaultController() const;
    ControllerType* getDefaultController();
    

    private:
    ControllersMap known_controllers; ///< Map containing all the ControllerTypes.
    std::map<std::string, ControllerType*> SDL_mapping_to_controllerType;
    ControllerType keyboard;
    ControllerType default_controller;
};