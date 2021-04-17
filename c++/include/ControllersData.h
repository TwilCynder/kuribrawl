#pragma once
#include <map>
#include <string>
#include "ControllerType.h"

/**
 * @brief Class containing all data related to controllers.
 * Contains every supported ControllerType, associated with a string identifier.
 */

class ControllersData {
    public:
    std::map<std::string, ControllerType> known_controllers; ///< Map containing all the ControllerTypes.
    ControllerType& addController(const std::string& name);
    ControllerType* getController(const std::string& name);
};