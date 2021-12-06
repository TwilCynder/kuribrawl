#pragma once
#include <vector>
#include "SDL2/SDL_rect.h"
#include "Util/util.h"

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

    Frame();
};