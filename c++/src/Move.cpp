#include "Move.h"

Move::Move() : 
    animation(nullptr)
    //edge_behavior(EdgeBehavior::FALL),
    //land_behavior(LandBehavior::NORMAL),
    //end_behavior (EndBehavior::NORMAL)
{}

Move::Move(const char* anim_name_) : 
    anim_name(anim_name_)
{
    Move();
}