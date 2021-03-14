#pragma once
#include "SDL2/SDL.h"
#include "util.h"
#include "CollisionBoxes.h"
#include <memory>

using namespace Kuribrawl;

class Animation {
    friend class CurrentAnimation;
    public:

    struct Frame {
        SDL_Rect display;
        Vector origin;
        int duration;
        std::unique_ptr<Hurtbox[]> hurtboxes;
        std::unique_ptr<Hitbox[]> hitboxes;
    };

    Animation();
    Animation(SDL_Texture* spritesheet);
    Animation(SDL_Texture* spritesheet, int nFrames);
    ~Animation();

    void setSpritesheet(SDL_Texture* spritesheet);
    void initFrames(int n);
    bool is_initialized();
    int getNbFrames();
    Frame* getFrame(int n);
    void setBaseSpeed(double speed);

    void draw(SDL_Renderer* target, int x, int y, int frame);

    private:
    SDL_Texture* spritesheet; //image used as the spritesheet
    std::unique_ptr<Frame[]> frames;
    int nb_frames;
    SDL_Rect display;

    double base_speed;
};