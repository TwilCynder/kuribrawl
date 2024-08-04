#pragma once
#include "SDL2/SDL.h"
#include "CBoxVectors.h"
#include "AnimationEndAction.h"
#include "Types/Data.h"

using namespace Kuribrawl::Types;

class Animation;
class Fighter;

/**
 * @brief Object that can run an Animation.
 * Contains all the information needed for an Animation *while it is running*. A Current Animation object is not restrained to a specific Animation,
 * and can switch to another Animation completely at any time.
 */

class AnimationPlayerBase {
    public:

    AnimationPlayerBase();
    AnimationPlayerBase(const Animation* animation);

    void draw(SDL_Renderer* target, int x, int y) const; 
    void draw(SDL_Renderer* target, int x, int y, Kuribrawl::Side facing) const;

    bool is_initialized() const;
    bool is_finished() const;
    bool frameChanged() const;
    const Animation* getAnimation() const;
    void setAnimation(const Animation* anim);
    void setAnimation(const Animation* anim, double speed);
    void setSpeed(double speed);
    void advance(bool loop);

    protected:
    frame_index_t current_frame; ///< Index of the current frame.
    const Animation* model;   ///< The Animation that is running.
                                    /**< Pointer validity : can be invalidated if an Animation is deleted or moved (should not happend while a AnimationPlayer instance exists)*/
    
    bool animation_set; ///< True if the animation has changed since the last advance().
    bool finished; ///< True if the animation just finished (even if it looped). 
    bool over; ///< True if the animation is finished and didn't loop.
    bool frame_changed; ///< True if the frame just changed
                        /**<Guarantees that the current frame has been set, but does not guaranteed that it actuzlly changed (the old value may be the new value)*/
   
    
    private:
    int timeleft; ///<Time remaining *on the current frame*.
    double speed; ///<Current \ref Animation#base_speed "speed" at which the animation runs (will override the base speed of the Animation).

    int frame_multiplier; ///< Time each frame will be displayed for.
    double base_carry; ///< If the total duration is not a multiple of the number of frames, certain frames will be displayed one frame-time longer than others. This calue is used to calculate which ones.
    double current_carry; ///< If the total duration is not a multiple of the number of frames, certain frames will be displayed one frame-time longer than others. This calue is used to calculate which ones.

    void nextFrame(bool loop);
    void reset();
    //void startFrame();
    void init();
    void start();
};

class AnimationPlayer : public AnimationPlayerBase, public AnimationEndActionOwner<Animation> {
    //might need to make this work like for entityanimationplayer, with a member indicating which end_action to use
    //and ::triggerEndAction etc
    public:
    template <typename... Args>
    AnimationPlayer(Args...args):
        AnimationPlayerBase(args...), AnimationEndActionOwner()
    {}

    void advance();
    void advance(AnimationEndAction<Animation>&);
};