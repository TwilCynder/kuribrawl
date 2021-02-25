#pragma once
#include "Animation.h"

class CurrentAnimation{
    public:

    CurrentAnimation();

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