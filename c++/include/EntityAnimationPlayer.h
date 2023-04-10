#pragma once
#include "AnimationPlayer.h"
#include "EntityFrame.h"
#include "AnimatedEntity.h"
#include "AnimationEndAction.h"

class EntityAnimation;
class EntityAnimationPlayer : public AnimationPlayerBase, public AnimationEndActionOwner<EntityAnimation> {
    public:

    EntityAnimationPlayer();
    EntityAnimationPlayer(EntityAnimation* animation);

    const std::vector<Hurtbox>& getHurtboxes() const;
    const std::vector<Hitbox>&  getHitboxes() const;
    const EntityFrame::FrameMovement& getFrameMovement() const;

    void setAnimation(const EntityAnimation* anim);
    void setAnimation(const EntityAnimation* anim, double speed);
    int advance();
    
    const EndAction& resolveEndAction() const;
    
    const EntityAnimation* getAnimation() const;

    template <typename... Args> requires EntityAnimationEndActionValidArg<EntityAnimation, Args...>
    void setEndAction(Args... args){
        end_action.set(args...);
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

    using AnimationPlayerBase::getAnimation;
    using AnimationPlayerBase::setAnimation;
};