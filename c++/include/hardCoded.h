#pragma once
#include "ControllersData.h"
#include "GameData.h"
#include "app.h"

/**
 * @brief Functions related to initialization and creation of hard-coded data.
 * 
 */
namespace HardCoded {
    void initControllersData(ControllersData&);
    void initGameData(App*);

    /**
     * @brief The default controller type
     * @todo REMOVE THIS when we don't need to potentially init PlayerFighters with the default controller type
     */
    extern ControllerType* default_controller_type;
}