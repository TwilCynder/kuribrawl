#include "Entity.h"

Entity::Entity() : 
    euid(makeEUID())
{
}

static uint32_t currentEUID = 0;
uint32_t Entity::makeEUID(){
    return currentEUID++;
}

uint32_t Entity::getEUID() const {
    return euid;
}