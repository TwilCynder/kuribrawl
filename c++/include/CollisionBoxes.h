#pragma once 

struct CollisionBox {
    int x;
    int y;
    int w;
    int h;
    };

struct Hurtbox : CollisionBox {
    enum Type {
        NORMAL,
        PROTECTED,
        INVINCIBLE,
        INTANGIBLE
    } type;
};

struct Hitbox : CollisionBox {
    enum Type{
        DAMAGE,
        GRAB,
        WIND,
        SPECIAL
    } type;
    double damage;
    double base_knockback;
    double scalink_knockback;
    float angle;
    int hit;
    int priority;
};