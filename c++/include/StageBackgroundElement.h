#pragma once

#include "Display/AnimationPlayer.h"
#include "Display/Drawable.h"

class StageBackgroundElementModel;
class SDL_Renderer;

class StageBackgroundElement : public Drawable {
    const StageBackgroundElementModel& model;
    AnimationPlayer animation_player;

    public:
    StageBackgroundElement(const StageBackgroundElementModel& model);

    const StageBackgroundElementModel& getModel() const;

    void advanceAnimation();
    void draw(SDL_Renderer*, const Camera&) const;

};