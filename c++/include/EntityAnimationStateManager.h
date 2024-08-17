#pragma once

#include "Display/AnimationPlayer.h"
#include "GameplayAnimationBehavior.h"


struct EntityAnimationStateManager : public AnimationPlayerStateManagerBase {
    using GAB = GameplayAnimationBehavior;

    GAB::LandingBehaviorWindows::const_iterator currentLandingWindow;

    public:
    void animationStarted(const EntityAnimation&);
    void frameChanged(const EntityAnimation&, frame_index_t frame_index);
};