#pragma once

#include "Display/AnimationPlayer.h"
#include "GameplayAnimationBehavior.h"


class EntityAnimationStateManager : public AnimationPlayerStateManagerBase {
    using GAB = GameplayAnimationBehavior;

    GAB::LandingBehaviorWindows::const_iterator currentLandingWindow;

    public:
    void animationStarted();
    void frameChanged();
};