#pragma once

#include "AnimationPlayer.h"
#include "Drawable.h"

class StageBackgroundElementModel;
class SDL_Renderer;

class StageBackgroundElement : public Drawable {
    const StageBackgroundElementModel& model;
    AnimationPlayer animation_player;

    public:
    StageBackgroundElement(const StageBackgroundElementModel& model);

    const StageBackgroundElementModel& getModel() const;

    void advanceAnimation();
    void draw(SDL_Renderer*) const;

};