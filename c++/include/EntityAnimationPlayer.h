#pragma once
#include "AnimationPlayer.h"
#include "EntityFrame.h"
#include "AnimatedEntity.h"

class EntityAnimation;
class EntityAnimationPlayer : public AnimationPlayer {
    public:

    EntityAnimationPlayer();
    EntityAnimationPlayer(EntityAnimation* animation);

    const std::vector<Hurtbox>& getHurtboxes() const;
    const std::vector<Hitbox>&  getHitboxes() const;
    const EntityFrame::FrameMovement& getFrameMovement() const;

    void setAnimation(const EntityAnimation* anim);
    void setAnimation(const EntityAnimation* anim, double speed);
    int advance();

    const EntityAnimation* getAnimation() const;

    private:

    using AnimationPlayer::getAnimation;
    using AnimationPlayer::setAnimation;
};