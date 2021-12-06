#pragma once
#include <vector>
#include "SDL2/SDL_rect.h"
#include "CollisionBoxes.h"
#include "Util/util.h"

using namespace Kuribrawl;

struct EntityFrame{
    using FrameMovementAxis = struct {
        bool enabled;
        bool set_speed; //if false speed is added
        bool whole_frame; //if false, set/added only when transitioning to this frame
        double value;
    };
    using FrameMovement = VectorT<FrameMovementAxis>;

    std::vector<Hurtbox> hurtboxes; 
    std::vector<Hitbox> hitboxes;
    FrameMovement movement;  ///< Speed used to modify the speed of the entity that owns the AnimationPlayer playing this Animation.
    EntityFrame();
};