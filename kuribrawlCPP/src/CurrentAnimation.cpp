#pragma once

#include "CurrentAnimation.h"
#include "Debug.h"

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
    return !!model;
}

void CurrentAnimation::setAnimation(Animation* anim){
    this->model = anim;
}

void CurrentAnimation::draw(SDL_Renderer* target, int x, int y){
    if (!model){
        throw KBFatal("Tried to draw un-initalized CAnimation");
    }

    model->draw(target, x, y, current_frame);
}