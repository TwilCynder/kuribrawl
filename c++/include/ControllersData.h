#pragma once
#include <map>
#include <string>
#include "ControllerType.h"

#define MAPPING_ID_LENGTH 32

/**
 * @brief Class containing all data related to controllers.
 * Contains every supported ControllerType, associated with a string identifier.
 */

class ControllersData {
    using ControllersMap = std::map<std::string, ControllerType>;
    using ControllerMapKVP = ControllersMap::value_type;

    public:
    ControllerType& addController(const std::string& name);
    ControllerType* getController(const std::string& name);
    void mapSDLMappingToControllerType(const char* mapping_string, ControllerType* ct);
    ControllerType* getControllerFromMapping(const char* mapping_string) const;

    private:
    ControllersMap known_controllers; ///< Map containing all the ControllerTypes.
    std::map<std::string, ControllerType*> SDL_mapping_to_controllerType;
};