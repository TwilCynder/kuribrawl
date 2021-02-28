#pragma once
#include "SDL2/SDL.h"
#include "util.h"
#include <memory>

using namespace Kuribrawl;

class Animation {
    public:
    struct CollisionBox {
        int x;
        int y;
        int w;
        int h;
        };

    struct Hurtbox : CollisionBox {
        bool invincible;
    };

    struct Hitbox : CollisionBox {
        double damage;
        double base_knockback;
        double scalink_knockback;
        float angle;
        int hit;
        int priority;
    };

    struct Frame {
        SDL_Rect display;
        Vector origin;
        int duration;
        std::unique_ptr<Hurtbox[]> hurtboxes;
        std::unique_ptr<Hitbox[]> hitboxes;
    };

    Animation();
    Animation(SDL_Texture* spritesheet);
    ~Animation();

    void setSpritesheet(SDL_Texture* spritesheet);
    void initFrames(int n);

    void draw(SDL_Renderer* target, int x, int y, int frame);

    private:
    SDL_Texture* spritesheet;
    std::unique_ptr<Frame[]> frames;
    double base_speed;
    int nb_frames;
    SDL_Rect display;
};