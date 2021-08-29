#include "ControllersData.h"
#include "Debug.h"


/**
 * @brief Returns a ControllerType.
 * @param name the identifier of a controller type.
 * @return ControllerType* a pointer to the ControllerType or NULL if the specified identifier doesn't match with any known ControllerType.
 * \todo Replace pointers returned by getController with a reference
 */

ControllerType* ControllersData::getController(const std::string& name){
    auto it = known_controllers.find(name);
    if (it == known_controllers.end()){
        throw KBFatal("Controller not found : " + name);
    } else {
        return &(it->second);
    }
    return nullptr;
}

/**
 * @brief Adds a new ControllerType.
 * Throws if the ControllerType already exists.
 * @param name identifier of the created ControllerType.
 * @return ControllerType& a reference to the created ControllerType. 
 */

ControllerType& ControllersData::addController(const std::string& name){
    auto [node, sucess] = known_controllers.try_emplace(name);
    if (!sucess){
        throw KBFatal("Could not add Controler " + name);
    } else {
        return node->second;
    }
}

void ControllersData::mapSDLMappingToControllerType(const char* mapping_string, ControllerType* ct){
    SDL_mapping_to_controllerType.emplace(std::string(mapping_string, MAPPING_ID_LENGTH), ct);
}

ControllerType* ControllersData::getControllerFromMapping(const char* mapping_string) const{
    std::string mapping_id(mapping_string, MAPPING_ID_LENGTH);

    auto it = SDL_mapping_to_controllerType.find(mapping_id);
    if (it == SDL_mapping_to_controllerType.end()){
        return NULL;
    }
    return it->second;
}