#include "Controllers.h"
#include "Port.h"

#define MAPPING_NORMAL_SIZE 32
#define isKeyboard (joystick_id == JOSTICKID_KEYBOARD)

void ControllersManager::removeController(int id){
    if (id > NB_CONTROLLERS) throw KBFatal("Attemp to remove controller with id above the limit");
    controllers[id] = 0;
}