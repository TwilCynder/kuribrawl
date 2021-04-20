#pragma once
#include "SDL2/SDL.h"

class Animation;
class Fighter;

/**
 * @brief Object that can run an Animation. 
 * Contains all the information needed for an Animation *while it is running*. A Current Animation object is not restrained to a specific Animation, 
 * and can switch to another Animation completely at any time.
 */

class CurrentAnimation{
    public:

    CurrentAnimation();
    CurrentAnimation(Animation* animation);
    void draw(SDL_Renderer* target, int x, int y);
    bool is_initialized();
    bool is_finished();
    void setAnimation(Animation* anim);
    void setAnimation(Animation* anim, double speed);
    void setSpeed(double speed);
    void advance();

    private:
    Animation* model;   ///< The Animation that is running.
                        /**< Pointer validity : can be invalidated if an Animation is deleted or moved (should not happend while a CurrentAnimation instance exists)*/

    int current_frame; ///< Index of the current frame.
    int timeleft; ///<Time remaining *on the current frame*.
    double speed; ///<Current \ref Animation#base_speed "speed" at which the animation runs (will override the base speed of the Animation).

    int frame_multiplier; ///< Time each frame will be displayed for.
    double base_carry; ///< If the total duration is not a multiple of the number of frames, certain frames will be displayed one frame-time longer than others. This calue is used to calculate which ones.
    double current_carry; ///< If the total duration is not a multiple of the number of frames, certain frames will be displayed one frame-time longer than others. This calue is used to calculate which ones.
    bool finished; ///< True if the animation just finished (even if it looped).
    Fighter* owner; ///< Entity running this animation. Currently restricted to \ref Fighter "Fighters".

    void nextFrame();
    void reset();
    void startFrame();
    void init();
    void start();
};