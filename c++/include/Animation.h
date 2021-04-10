#pragma once
#include <memory>
#include "SDL2/SDL.h"
#include "Frame.h"
#include "util.h"

using namespace Kuribrawl;

class Animation {
    friend class CurrentAnimation;

    public:

    Animation();
    Animation(SDL_Texture* spritesheet);
    Animation(SDL_Texture* spritesheet, int nFrames);
    ~Animation();

    void setSpritesheet(SDL_Texture* spritesheet);
    SDL_Texture* getSpritesheet() const ;
    bool is_initialized();
    int getNbFrames();
    Frame* getFrame(int n); //Pointer validity : frames are stored in a unique pointer, can't be invalid as long as returns a frame of this animation
    void setBaseSpeed(double speed);

    void draw(SDL_Renderer* target, int x, int y, int frame);

    //Construction
    void initFrames(int n);
    Hurtbox& addHurtbox();
    Hitbox& addHitbox();

    private:
    SDL_Texture* spritesheet; //image used as the spritesheet
    std::unique_ptr<Frame[]> frames;
    int nb_frames;
    Vector display;

    double base_speed;
};