///\todo Animation* -> Animation&

#include <math.h>
#include "Display/AnimationPlayer.h"
#include "Display/Animation.h"
#include "KBDebug/Debug.h"
#include "Display/Frame.h"
#include "Display/AnimationPlayer.h"
#include "EntityAnimationStateManager.h"
#include "Display/EntityAnimation.h"

/**
 * @brief Construct a new Current Animation object.
 * Creates it without an Animation to run, so it is unusable until an Animation is set.
 */

template <typename A, typename StateManager>
AnimationPlayerBase<A, StateManager>::AnimationPlayerBase():
    AnimationEndActionOwner<A>(),
    current_frame(0),
    model(nullptr),
    animation_set(),
    finished(false),
    over(false),
    frame_changed(false)
{
}

/**
 * @brief Construct a new Current Animation object.
 *
 * @param animation the Animation that will be ran at first.
 */
template <typename A, typename StateManager>
AnimationPlayerBase<A, StateManager>::AnimationPlayerBase(const A* animation):
    AnimationPlayerBase()
{
    setAnimation(animation);
}

/**
 * @brief Returns wether the animation is actually running and has an Animation to run.
 *
 * @return true if there is an Animation to be ran.
 * @return false otherwise
 */

template <typename A, typename StateManager>
bool AnimationPlayerBase<A, StateManager>::is_initialized()const{
    return model && model->is_initialized();
}

template <typename A, typename StateManager>
const A* AnimationPlayerBase<A, StateManager>::getAnimation() const {
    return model;
}

/**
 * @brief Retur whether the animation just finished (even if it looped).
 *
 * @return finished or not.
 */
template <typename A, typename StateManager>
bool AnimationPlayerBase<A, StateManager>::is_finished() const {
    return finished;
}

template <typename A, typename StateManager>
bool AnimationPlayerBase<A, StateManager>::frameChanged() const {
    return frame_changed;
}

template <typename A, typename StateManager>
void AnimationPlayerBase<A, StateManager>::changeAnimation(const A *anim){
    changeAnimation(anim, anim->base_speed);
}

template <typename A, typename StateManager>
void AnimationPlayerBase<A, StateManager>::changeAnimation(const A *anim, double speed_){
    if (!anim) return;
    model = anim;
    setSpeed(speed_);
    animation_set = true;
}

template <typename A, typename StateManager>
void AnimationPlayerBase<A, StateManager>::setAnimationPaused(const A *anim){
    changeAnimation(anim);
    running = false;
}

template <typename A, typename StateManager>
void AnimationPlayerBase<A, StateManager>::setAnimationPaused(const A *anim, double speed_){
    changeAnimation(anim, speed_);
    running = false;
}

/**
 * @brief Sets the ran Animation.
 * Delegates to setAnimation(Animation*, double) with the Animation's default speed.
 * @param anim
 */

template <typename A, typename StateManager>
void AnimationPlayerBase<A, StateManager>::setAnimation(const A* anim){
    setAnimation(anim, anim->base_speed);
}

/**
 * @brief Sets the ran Animation, and the \ref Animation#base_speed "speed" at which it will be ran.
 *
 * @param anim
 * @param speed_ \ref Animation#base_speed "speed" that will be used when advancing this animation.
 */

template <typename A, typename StateManager>
void AnimationPlayerBase<A, StateManager>::setAnimation(const A* anim, double speed_){
    changeAnimation(anim, speed_);
    start_();
}

template <typename A, typename StateManager>
void AnimationPlayerBase<A, StateManager>::start_(){
    initAnim();
    initRun();
    running = true;
}

template <typename A, typename StateManager>
void AnimationPlayerBase<A, StateManager>::start(){
    if (!running){
        start_();
    }
}

template <typename A, typename StateManager>
void AnimationPlayerBase<A, StateManager>::pause(){
    running = false;
}

/**
 * @brief Sets the current \ref Animation#base_speed "speed".
 *
 * @param speed_ the \ref Animation#base_speed "speed" that will be used from now on.
 * For speeds that do not make each frame displayed the same amount of time, using this method while the animation has already advanced at least one frame will result in imprecisions.
 */

template <typename A, typename StateManager>
void AnimationPlayerBase<A, StateManager>::setSpeed(double speed_){

    if (speed_){
        speed = speed_;
    } else {
        speed = model->base_speed;
    }

    if (speed == 0){
        throw KBFatal("Animation set with a null speed");
    }
    
    double ratio;
    int cutFrames;
    if (speed > 1){ //the speed must be understood as a total number of frame
        ratio = speed / model->nb_frames;
        frame_multiplier = ceil(ratio);
        if (model->nb_frames > 1 && (double)frame_multiplier != ratio){
            cutFrames = (frame_multiplier * model->nb_frames) - speed;
            base_carry = (1.0 / (model->nb_frames - 1)) * cutFrames;
        } else {
            base_carry = 0;
        }
    } else {
        frame_multiplier = 1 / speed;
        base_carry = 0;
    }
}

/**
 * @brief Initializes this Current Animation, to make it ready to start if it has an Animation.
 *
 */

template <typename A, typename StateManager>
void AnimationPlayerBase<A, StateManager>::initAnim(){
    over = false;
    finished = false;


}

/**
 * @brief resets this Current Animation to the initial state of the current Animation.
 */

template <typename A, typename StateManager>
void AnimationPlayerBase<A, StateManager>::reset(){
    current_frame = 0;
    frame_changed = true;
    current_carry = 0;
}

/**
 * @brief Function called when the Current Animation is started.
 *
 */

template <typename A, typename StateManager>
void AnimationPlayerBase<A, StateManager>::initRun(){
    reset();

    Frame* f = &(model->frames[current_frame]);
    if (f->duration){
        timeleft = f->duration;
    } else if (speed != -1){
        timeleft = frame_multiplier;
    }

    state_manager.animationStarted(*model);
}

/**
 * @brief Advances the animation, which means doing all that need to be done at each iteration of the main loop for the animation to progress (at the right speed)
 *
 */

template <typename A, typename StateManager>
void AnimationPlayerBase<A, StateManager>::advance(bool loop){
    if (speed != -1 && is_initialized() && running){
        
        if (animation_set){
            frame_changed = true;
            animation_set = false;
        } else  {
            finished = false;
            frame_changed = false;
        }

        timeleft--;
        if (timeleft <= 0){
            current_carry += base_carry;
            nextFrame(loop);
        }

    }
}

/**
 * @brief Changes the current frame (and updates all the attributes that depend on the current frame)
 * Updates the timeleft for the current frame, and the carry if needed.
 */

template <typename A, typename StateManager>
void AnimationPlayerBase<A, StateManager>::nextFrame(bool loop){
    if (!model->loop && over) return;
    
    if (current_frame < model->nb_frames - 1){
        current_frame++;
        frame_changed = true;
        state_manager.frameChanged(*model, current_frame);
    } else {
        finished = true;
        if (loop){
            reset();
            initRun();
        } else {
            over = true;
        }
        return;
    }

    Frame* f = &(model->frames[current_frame]);
    if (f->duration){
        timeleft = f->duration;
    } else if (speed != 1 && speed != -1){
        timeleft = frame_multiplier;
        if (current_carry >= 1){
            current_carry--;
            timeleft--;
        }
    }
}

/**
 * @brief Draws this Current Animation.
 * Draws the current frame of the currently ran Animation.
 * @param target the renderer it will be drawn to.
 * @param x x position on the destination.
 * @param y y position on the destination.
 */
template <typename A, typename StateManager>
void AnimationPlayerBase<A, StateManager>::draw(SDL_Renderer* target, int x, int y) const {
    if (!model){
        throw KBFatal("Tried to draw non-initalized AnimationPlayerBase");
    }

    model->draw(target, x, y, current_frame);
}

template <typename A, typename StateManager>
void AnimationPlayerBase<A, StateManager>::draw(SDL_Renderer* target, int x, int y, Kuribrawl::Side facing) const {
    if (!model){
        throw KBFatal("Tried to draw non-initalized AnimationPlayerBase");
    }

    model->draw(target, x, y, current_frame, facing);
}

template <typename A, typename StateManager>
void AnimationPlayerBase<A, StateManager>::advance(AnimationEndAction<A>& end_action_){
    advance(end_action_.mode == EndAction::Mode::REPEAT);
}

template <typename A, typename StateManager>
void AnimationPlayerBase<A, StateManager>::advance(){
    advance(this->end_action);
}

void AnimationPlayerStateManagerBase::animationStarted(const Animation&){}

void AnimationPlayerStateManagerBase::frameChanged(const Animation&, frame_index_t frame_index){}

void AnimationPlayerStateManagerBase::animationEnded(const Animation&){}

template class AnimationPlayerBase<Animation, AnimationPlayerStateManagerBase>;
template class AnimationPlayerBase<EntityAnimation, EntityAnimationStateManager>;