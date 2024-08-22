#pragma once
#include "SDL2/SDL.h"
#include "CBoxVectors.h"
#include "AnimationEndAction.h"
#include "Types/Data.h"
#include "Display/Animation.h"
#include "Display/AnimationEndAction.h"

using namespace Kuribrawl::Types;

struct AnimationPlayerStateManagerBase {
    void animationStarted(const Animation&);
    void frameChanged(const Animation&, frame_index_t frame_index);
    void animationEnded(const Animation&);
};


/**
 * @brief Object that can run an Animation.
 * Contains all the information needed for an Animation *while it is running*. A Current Animation object is not restrained to a specific Animation,
 * and can switch to another Animation completely at any time.
 */
template <typename A, typename StateManager>
//requires std::is_base_of_v<AnimationBase, A> && std::is_base_of_v<AnimationPlayerStateManagerBase, StateManager>
class AnimationPlayerBase : public AnimationEndActionOwner<A>{
    public:
    using EndAction = AnimationEndAction<A>;

    AnimationPlayerBase();
    AnimationPlayerBase(const A* animation);

    void draw(SDL_Renderer* target, int x, int y) const; 
    void draw(SDL_Renderer* target, int x, int y, Kuribrawl::Side facing) const;

    const A* getAnimation() const;
    void setAnimationPaused(const A* anim);
    void setAnimationPaused(const A* anim, double speed);
    void setAnimation(const A* anim);
    void setAnimation(const A* anim, double speed);
    void setAnimationPaused(const A* anim, const EndAction&);
    void setAnimationPaused(const A* anim, double speed, const EndAction&);
    void setAnimation(const A* anim, const EndAction&);
    void setAnimation(const A* anim, double speed, const EndAction&);

    inline void setEndAction(const EndAction&);
    template <typename... Args> requires AnimationEndActionValidArg<A, Args...>
    void setEndAction(Args... args){
        this->end_action.set(args...);
        override_end_action = true;
    }
    inline void unsetEndAction();
    const EndAction& getPlayerEndAction() const;
    const EndAction& getFinalEndAction() const;

    void setSpeed(double speed);
    bool is_initialized() const;
    bool is_finished() const;
    bool frameChanged() const;

    void start();
    void pause();
    int advance(const EndAction&); 
    int advance();

    protected:
    inline void changeAnimation(const A* anim);
    inline void changeAnimation(const A* anim, double speed);

    inline void setAnimationPaused_(const A* anim);
    inline void setAnimationPaused_(const A* anim, double speed);
    inline void setAnimation_(const A* anim);
    inline void setAnimation_(const A* anim, double speed);

    const EndAction& resolveEndAction() const;

    void start_();

    frame_index_t current_frame; ///< Index of the current frame.
    const A* model;   ///< The Animation that is running.
                                    /**< Pointer validity : can be invalidated if an Animation is deleted or moved (should not happend while a AnimationPlayer instance exists)*/
    
    bool animation_set; ///< True if the animation has changed since the last advance().
    bool finished; ///< True if the animation just finished (even if it looped). 
    bool over; ///< True if the animation is finished and didn't loop.
    bool frame_changed; ///< True if the frame just changed
                        /**<Guarantees that the current frame has been set, but does not guaranteed that it actuzlly changed (the old value may be the new value)*/
    bool running; ///< If false, the animation doesnt progress

    bool override_end_action;

    StateManager state_manager;

    private:
    int timeleft; ///<Time remaining *on the current frame*.
    double speed; ///<Current \ref Animation#base_speed "speed" at which the animation runs (will override the base speed of the Animation).

    int frame_multiplier; ///< Time each frame will be displayed for.
    double base_carry; ///< If the total duration is not a multiple of the number of frames, certain frames will be displayed one frame-time longer than others. This calue is used to calculate which ones.
    double current_carry; ///< If the total duration is not a multiple of the number of frames, certain frames will be displayed one frame-time longer than others. This calue is used to calculate which ones.

    void nextFrame(bool loop);
    void reset();
    //void startFrame();
    void initAnim();
    void initRun();
};

using AnimationPlayer = AnimationPlayerBase<Animation, AnimationPlayerStateManagerBase>;