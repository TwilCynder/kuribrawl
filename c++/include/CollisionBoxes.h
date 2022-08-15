#pragma once 

/**
 * @brief Basically a rectangle, that will be used as a collision area for hit detection
 * Coordinates works like follows :
 * Position relative to Fighter position, with a **down-top** y axis  (which uses **down-top** coordinates too (relative to the center point of the stage))  
 * Position is the **top-left** corner of the box.
 */
struct CollisionBox {
    int x;
    int y;
    int w;
    int h;

    int getRealXPos(int facing) const;
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
    using HitID = int;
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
    HitID hit;
    int priority;
};