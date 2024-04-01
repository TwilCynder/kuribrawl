#include "CollisionBoxes.h"

int CollisionBox::getRealXPos(Kuribrawl::Side facing) const{
    return (facing == Kuribrawl::Side::RIGHT) ? x : -x - w;
}