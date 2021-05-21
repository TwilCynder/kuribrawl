#include <math.h>
#include "CurrentAnimation.h"
#include "EntityAnimation.h"
#include "Debug.h"
#include "Frame.h"

/**
 * @brief Construct a new Current Animation object.
 * Creates it without an Animation to run, so it is unusable until an Animation is set.
 */

CurrentAnimation::CurrentAnimation():
    current_frame(0)
{
}

/**
 * @brief Construct a new Current Animation object.
 *
 * @param animation the Animation that will be ran at first.
 */

CurrentAnimation::CurrentAnimation(EntityAnimation* animation):
    CurrentAnimation()
{
    setAnimation(animation);
}

/**
 * @brief Returns wether the animation is actually running and has an Animation to run.
 *
 * @return true if there is an Animation to be ran.
 * @return false otherwise
 */

bool CurrentAnimation::is_initialized()const{
    return model && model->is_initialized();
}

/**
 * @brief Retur whether the animation just finished (even if it looped).
 *
 * @return finished or not.
 */
bool CurrentAnimation::is_finished(){
    return finished;
}

/**
 * @brief Sets the ran Animation.
 * Delegates to setAnimation(Animation*, double) with the Animation's default speed.
 * @param anim
 */

void CurrentAnimation::setAnimation(const EntityAnimation* anim){
    setAnimation(anim, anim->base_speed);
}

/**
 * @brief Sets the ran Animation, and the \ref Animation#base_speed "speed" at which it will be ran.
 *
 * @param anim
 * @param speed_ \ref Animation#base_speed "speed" that will be used when advancing this animation.
 */

void CurrentAnimation::setAnimation(const EntityAnimation* anim, double speed_){
    model = anim;
    init();
    setSpeed(speed_);
    start();
}

/**
 * @brief Sets the current \ref Animation#base_speed "speed".
 *
 * @param speed_ the \ref Animation#base_speed "speed" that will be used from now on.
 * For speeds that do not make each frame displayed the same amount of time, using this method while the animation has already advanced at least one frame will result in imprecisions.
 */

void CurrentAnimation::setSpeed(double speed_){
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
            base_carry = (1 / (model->nb_frames - 1)) * cutFrames;
        }
    } else {
        frame_multiplier = 1 / speed;
    }
}

/**
 * @brief Initializes this Current Animation, to make it ready to start if it has an Animation.
 *
 */

void CurrentAnimation::init(){
    reset();
    finished = 0;
}

/**
 * @brief resets this Current Animation to the initial state of the current Animation.
 */

void CurrentAnimation::reset(){
    current_frame = 0;
    current_carry = 0;
}

/**
 * @brief Function called when the Current Animation is started.
 *
 */

void CurrentAnimation::start(){
    Frame* f = &(model->frames[current_frame]);
    if (f->duration){
        timeleft = f->duration;
    } else if (speed != 1 && speed != -1){
        timeleft = frame_multiplier;
    }
}

/**
 * @brief Advances the animation, which means doing all that need to be done at each iteration of the main loop for the animation to progress (at the right speed)
 *
 */

void CurrentAnimation::advance(){
    if (speed != -1 && is_initialized()){
        timeleft--;
        if (timeleft <= 0){
            current_carry += base_carry;
            nextFrame();
        }
    }
}

/**
 * @brief Changes the current frame (and updates all the attributes that depend on the current frame)
 * Updates the timeleft for the current frame, and the carry attriutes if needed.
 */

void CurrentAnimation::nextFrame(){
    finished = false;
    current_frame++;

    if (current_frame >= model->nb_frames){
        finished = true;
        const EntityAnimation* next = model->getNextAnimation();
        if (next != nullptr){
            setAnimation(next);
        } else {
            reset();
            start();
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

void CurrentAnimation::draw(SDL_Renderer* target, int x, int y){
    if (!model){
        throw KBFatal("Tried to draw non-initalized CurrentAnimation");
    }

    model->draw(target, x, y, current_frame);
}

void CurrentAnimation::draw(SDL_Renderer* target, int x, int y, int facing){
    if (!model){
        throw KBFatal("Tried to draw non-initalized CurrentAnimation");
    }

    model->draw(target, x, y, current_frame, facing);
}

const std::vector<Hurtbox>& CurrentAnimation::getHurtboxes() const {
    return model->getHurtboxes(current_frame);
}