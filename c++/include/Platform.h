#pragma once

#include <memory>
#include "Util/structures.h"
#include "Display/AnimationPlayer.h"
#include "Display/Drawable.h"
#include "Ground.h"

class PlatformModel;
class SDL_Renderer;

class Platform : public Drawable, public Ground {
    const PlatformModel& model;
    std::unique_ptr<AnimationPlayer> animation_player;
    Kuribrawl::Vector position;

    public:
    Platform(const PlatformModel&);

    void advanceAnimation();
    void draw(SDL_Renderer*, const Camera&) const;
    virtual bool isTraversable() const;
    bool hasAnimation() const;
    const Kuribrawl::Vector& getPosition() const;
    const int getWidth() const;
    const int getHalfWidth() const;
};