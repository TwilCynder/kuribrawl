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

    int advance();
    
    const EndAction& resolveEndAction() const;
    
    const EntityAnimation* getAnimation() const;

    template <typename... Args> requires EntityAnimationEndActionValidArg<EntityAnimation, Args...>
    void setEndAction(Args... args){
        end_action.set(args...);
        override_end_action = true;
    }

    template <typename... Args>
    void setAnimation(const EntityAnimation* anim, Args... args){
        setAnimation(anim);
        setEndAction(args...);
    }
    template <typename... Args>
    void setAnimation(const EntityAnimation* anim, double speed, Args... args){
        setAnimation(anim, speed);
        setEndAction(args...);
    }


    private:

    bool override_end_action;
};