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
}