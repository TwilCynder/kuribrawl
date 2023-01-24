#pragma once

#include <memory>
#include "AnimationPlayer.h"
#include "Drawable.h"

class PlatformModel;
class SDL_Renderer;

class Platform : public Drawable {
    const PlatformModel& model;
    std::unique_ptr<AnimationPlayer> animation_player;

    public:
    Platform(const PlatformModel&);

    void advanceAnimation();
    void draw(SDL_Renderer*, const Camera&) const;
    bool hasAnimation() const;
};