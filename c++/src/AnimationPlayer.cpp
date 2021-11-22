///\todo Animation* -> Animation&

#include <math.h>
#include "AnimationPlayer.h"
#include "Animation.h"
#include "Debug.h"
#include "Frame.h"

/**
 * @brief Construct a new Current Animation object.
 * Creates it without an Animation to run, so it is unusable until an Animation is set.
 */

AnimationPlayer::AnimationPlayer():
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

AnimationPlayer::AnimationPlayer(Animation* animation):
    AnimationPlayer()
{
    setAnimation(animation);
}

/**
 * @brief Returns wether the animation is actually running and has an Animation to run.
 *
 * @return true if there is an Animation to be ran.
 * @return false otherwise
 */

bool AnimationPlayer::is_initialized()const{
    return model && model->is_initialized();
}

const Animation* AnimationPlayer::getAnimation() const {
    return model;
}

/**
 * @brief Retur whether the animation just finished (even if it looped).
 *
 * @return finished or not.
 */
bool AnimationPlayer::is_finished() const {
    return finished;
}

bool AnimationPlayer::frameChanged() const {
    return frame_changed;
}

/**
 * @brief Sets the ran Animation.
 * Delegates to setAnimation(Animation*, double) with the Animation's default speed.
 * @param anim
 */

void AnimationPlayer::setAnimation(const Animation* anim){
    setAnimation(anim, anim->base_speed);
}

/**
 * @brief Sets the ran Animation, and the \ref Animation#base_speed "speed" at which it will be ran.
 *
 * @param anim
 * @param speed_ \ref Animation#base_speed "speed" that will be used when advancing this animation.
 */

void AnimationPlayer::setAnimation(const Animation* anim, double speed_){
    if (!anim) return;
    model = anim;
    init();
    setSpeed(speed_);
    start();

    animation_set = true;
}

/**
 * @brief Sets the current \ref Animation#base_speed "speed".
 *
 * @param speed_ the \ref Animation#base_speed "speed" that will be used from now on.
 * For speeds that do not make each frame displayed the same amount of time, using this method while the animation has already advanced at least one frame will result in imprecisions.
 */

void AnimationPlayer::setSpeed(double speed_){
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

void AnimationPlayer::init(){
    over = false;
    finished = false;
    reset();
}

/**
 * @brief resets this Current Animation to the initial state of the current Animation.
 */

void AnimationPlayer::reset(){
    current_frame = 0;
    frame_changed = true;
    current_carry = 0;
}

/**
 * @brief Function called when the Current Animation is started.
 *
 */

void AnimationPlayer::start(){
    Frame* f = &(model->frames[current_frame]);
    if (f->duration){
        timeleft = f->duration;
    } else if (speed != -1){
        timeleft = frame_multiplier;
    }
}

/**
 * @brief Advances the animation, which means doing all that need to be done at each iteration of the main loop for the animation to progress (at the right speed)
 *
 */

void AnimationPlayer::advance(){
    if (speed != -1 && is_initialized()){
        
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
            nextFrame();
        }

    }
}

/**
 * @brief Changes the current frame (and updates all the attributes that depend on the current frame)
 * Updates the timeleft for the current frame, and the carry if needed.
 */

void AnimationPlayer::nextFrame(){
    if (!model->loop && over) return;
    
    if (current_frame < model->nb_frames - 1){
        current_frame++;
        frame_changed = true;
    } else {
        finished = true;
        if (model->loop){
            reset();
            start();
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

void AnimationPlayer::draw(SDL_Renderer* target, int x, int y){
    if (!model){
        throw KBFatal("Tried to draw non-initalized AnimationPlayer");
    }

    model->draw(target, x, y, current_frame);
}

void AnimationPlayer::draw(SDL_Renderer* target, int x, int y, int facing){
    if (!model){
        throw KBFatal("Tried to draw non-initalized AnimationPlayer");
    }

    model->draw(target, x, y, current_frame, facing);
}