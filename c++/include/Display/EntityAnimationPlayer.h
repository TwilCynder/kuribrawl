#pragma once
#include "AnimationPlayer.h"
#include "EntityFrame.h"
#include "EntityAnimationStateManager.h"
#include "Display/EntityAnimation.h"

using EntityAnimationPlayerBase = AnimationPlayerBase<EntityAnimation, EntityAnimationStateManager>;
class EntityAnimationPlayer : public EntityAnimationPlayerBase {
    public:

    EntityAnimationPlayer();
    EntityAnimationPlayer(const EntityAnimation* animation);

    const std::vector<Hurtbox>& getHurtboxes() const;
    const std::vector<Hitbox>&  getHitboxes() const;
    const EntityFrame::FrameMovement& getFrameMovement() const;
    const GameplayAnimationBehavior::EndingBehavior getEndBehavior() const;
    const GameplayAnimationBehavior::LandingBehavior* getLandingBehavior() const;
    /*
    template <typename... Args>
    void setAnimationWithEndAction(const EntityAnimation* anim, Args... args){
        setAnimation(anim);
        setEndAction(args...);
    }

    template <typename... Args>
    void setAnimationWithEndAction(const EntityAnimation* anim, double speed, Args... args){
        setAnimation(anim, speed);
        setEndAction(args...);
    }
    */

    private:

    //bool override_end_action;
};