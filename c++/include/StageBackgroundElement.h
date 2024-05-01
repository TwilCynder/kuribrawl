#pragma once

#include "Display/AnimationPlayer.h"
#include "DepthfulEntity.h"

class StageBackgroundElementModel;
class SDL_Renderer;

class StageBackgroundElement : public DepthfulEntity {
    const StageBackgroundElementModel& model;
    AnimationPlayer animation_player;

    public:
    StageBackgroundElement(const StageBackgroundElementModel& model);

    const StageBackgroundElementModel& getModel() const;

    void advanceAnimation();
    void draw(SDL_Renderer*, const Camera&) const;

};