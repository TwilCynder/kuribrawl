#include "Move.h"

Move::Move() : 
    edge_behavior(EdgeBehavior::FALL),
    land_behavior(LandBehavior::NORMAL),
    landing_lag(50),
    animation(nullptr)
{

}

Move::Move(const char* anim_name_) : 
    anim_name(anim_name_)
{
    Move();
}