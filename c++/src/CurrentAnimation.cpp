#include <math.h>
#include "CurrentAnimation.h"
#include "Debug.h"
#include "Frame.h"

CurrentAnimation::CurrentAnimation():
    current_frame(0)
{
    
}

CurrentAnimation::CurrentAnimation(Animation* animation):
    model(animation)
{
    CurrentAnimation();
}

bool CurrentAnimation::is_initialized(){
    return model && model->is_initialized();
}

void CurrentAnimation::setAnimation(Animation* anim){
    setAnimation(anim, anim->base_speed);
}

void CurrentAnimation::setAnimation(Animation* anim, double speed_){
    model = anim;
    init();
    setSpeed(speed_);
    start();
}

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

void CurrentAnimation::init(){
    reset();
    finished = 0;
}

void CurrentAnimation::reset(){
    current_frame = 0;
    current_carry = 0;
}

void CurrentAnimation::start(){
    Frame* f = &(model->frames[current_frame]);
    if (f->duration){
        timeleft = f->duration;
    } else if (speed != 1 && speed != -1){
        timeleft = frame_multiplier;
    }
}

void CurrentAnimation::advance(){
    if (speed != -1 && is_initialized()){
        timeleft--;
        if (timeleft <= 0){
            current_carry += base_carry;
            nextFrame();
        }
    }
}

void CurrentAnimation::nextFrame(){
    finished = false;
    current_frame++;

    if (current_frame >= model->nb_frames){
        finished = true;
        reset();
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

void CurrentAnimation::draw(SDL_Renderer* target, int x, int y){
    if (!model){
        throw KBFatal("Tried to draw non-initalized CurrentAnimation");
    }

    model->draw(target, x, y, current_frame);
}