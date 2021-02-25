#pragma once
#include <list>
#include "SDL2/SDL_image.h"
#include "util.h"

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
        Rect display;
        Vector origin;
        int duration;
    };

    Animation();

    private:
    SDL_Texture* spritesheet;
    std::list<Frame> frames;
    double base_speed;
    int nb_frames;
};