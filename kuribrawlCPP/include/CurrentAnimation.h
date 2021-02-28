#pragma once

#include "SDL2/SDL.h"
#include "Animation.h"

class CurrentAnimation{
    public:

    CurrentAnimation();
    CurrentAnimation(Animation* animation);
    void draw(SDL_Renderer* target, int x, int y);
    bool is_initialized();
    void setAnimation(Animation* anim);

    private:
    Animation* model;

    int current_frame;
    int timeleft; ///number of frames remaining *on the current frame*
    double speed; //current speed

    int frame_multiplier;
    double base_carry;
    double current_carry;

    bool finished;
};