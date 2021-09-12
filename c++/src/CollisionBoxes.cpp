#include "CollisionBoxes.h"

int CollisionBox::getRealXPos(int facing) const{
    return (facing == 1) ? x : -x - w;
}