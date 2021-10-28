#pragma once
#include "AnimationPlayer.h"
#include "Entity.h"

class EntityAnimation;
class EntityAnimationPlayer : public AnimationPlayer {
    public:

    EntityAnimationPlayer();
    EntityAnimationPlayer(EntityAnimation* animation);

    const std::vector<Hurtbox>& getHurtboxes() const;
    const std::vector<Hitbox>&  getHitboxes() const;

    void setAnimation(const EntityAnimation* anim);
    void setAnimation(const EntityAnimation* anim, double speed);

    const EntityAnimation* getAnimation() const;

    private:
    using AnimationPlayer::getAnimation;
    using AnimationPlayer::setAnimation;
    Entity* owner; ///< Entity running this animation. Currently restricted to \ref Fighter "Fighters".
};