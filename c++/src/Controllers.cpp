#include "Controllers.h"
#include "Port.h"

void ControllersManager::removeController(int id){
    if (id > NB_CONTROLLERS) throw KBFatal("Attemp to remove controller with id above the limit");
    controllers[id] = 0;
}