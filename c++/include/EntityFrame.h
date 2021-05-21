#pragma once
#include <vector>
#include "SDL2/SDL_rect.h"
#include "CollisionBoxes.h"
#include "util.h"

using namespace Kuribrawl;

struct EntityFrame{
    std::vector<Hurtbox> hurtboxes; 
    std::vector<Hitbox> hitboxes;
    VectorDouble movement;  ///< Speed used to modify the speed of the entity that owns the CurrentAnimation playing this Animation.
    Vector movement_type; //bit 1 : enabled/disabled | bit 2 : speed is set/added | bit 3 : when frame starts/during the whole frame

    EntityFrame();
};