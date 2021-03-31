#pragma once
#include "SDL2/SDL.h"
#include "Animation.h"

class Fighter;

class CurrentAnimation{
    public:

    CurrentAnimation();
    CurrentAnimation(Animation* animation);
    void draw(SDL_Renderer* target, int x, int y);
    bool is_initialized();
    void setAnimation(Animation* anim);
    void setAnimation(Animation* anim, double speed);
    void setSpeed(double speed);
    void advance();

    private:
    Animation* model;  //Pointer validity : can be invalidated if an Animation is deleted or moved (should not happend while a CurrentAnimation instance exists)

    int current_frame; //index of the current frame
    int timeleft; //number of frames remaining *on the current frame*
    double speed; //current speed

    int frame_multiplier; //number of real frames each frame will be displayed for
    double base_carry; 
    double current_carry;
    bool finished; //true if the animation just finished (even if it looped)
    Fighter* owner;

    void nextFrame();
    void reset();
    void startFrame();
    void init();
    void start();
};