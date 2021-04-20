#pragma once
#include <vector>
#include "SDL2/SDL_rect.h"
#include "CollisionBoxes.h"
#include "util.h"

using namespace Kuribrawl;

/**
 * @brief An Animation frame.
 * A Frame does not contain any visual information, as an Animation use a \ref Animation#spritesheet "spritesheet image" that contains all the frames of the animation.
 * A Frame object only contains the coordinates of every frame on the image.  
 * A Frame also contains informations on the effect of the Animation on gameplay when this frame is the current frame.
 */
struct Frame {
    SDL_Rect display;   ///< Rectangle on the spritesheet image that will be drawn when drawing this Frame.
    Vector origin;      ///< Position of the origin of this frame.\ When displaying this Animation, the origin point of the frame will be at the position that was specified. 
    int duration;       ///< Number of frame-times this Frame should be displayed for, if it shouldn't follow the normal Animation speed behavior.
    std::vector<Hurtbox> hurtboxes; 
    std::vector<Hitbox> hitboxes;
    VectorDouble movement;  ///< Speed used to modify the speed of the entity that owns the CurrentAnimation playing this Animation.
    Vector movement_type; //bit 1 : enabled/disabled | bit 2 : speed is set/added | bit 3 : when frame starts/during the whole frame

    Frame();
};