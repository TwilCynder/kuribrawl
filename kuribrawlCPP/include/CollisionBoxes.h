#pragma once 

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