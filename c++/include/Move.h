#pragma once
#include <string>

class EntityAnimation;

struct Move {
    enum struct EdgeBehavior {
        KEEP, ///< Do nothing, move keeps going while the fighter is now in the air
        FALL, ///< Interrupt the move and go in (air) IDLE state
        BLOCK, ///< Fighter is stopped at the edge while the move is occuring
        EXTEND ///< Move keeps going and fighter is still considered as on the the ground until the move end (will be a fucking pain to implement)
    };

    enum struct EndBehavior{
        NORMAL, ///< Go into IDLE state
        FREEFALL
    };

    Move();
    Move(const char* anim_name);

    std::string anim_name;
    const EntityAnimation* animation; //nullptr if no animation

    //Keep in mind that with the right edgeBehavior/landBehavior, any grounded move can be done airborne, and any aerial move can be done on the ground
    //EdgeBehavior edge_behavior;
    //LandBehavior land_behavior;
    //EndBehavior  end_behavior ;
};