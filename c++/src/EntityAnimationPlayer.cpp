#include "EntityAnimationPlayer.h"
#include "EntityAnimation.h"

#define model_ ((EntityAnimation*)model)

/**
 * @brief Construct a new Current Animation object.
 *
 * @param animation the Animation that will be ran at first.
 */

EntityAnimationPlayer::EntityAnimationPlayer(EntityAnimation* animation):
    AnimationPlayer((Animation*)animation)
{
}

const std::vector<Hurtbox>& EntityAnimationPlayer::getHurtboxes() const {
    return model_->getHurtboxes(current_frame);
}

const std::vector<Hitbox>& EntityAnimationPlayer::getHitboxes() const {
    return model_->getHitboxes(current_frame);
}

const EntityAnimation* EntityAnimationPlayer::getAnimation() const {
    return model_;
}

void EntityAnimationPlayer::setAnimation(const EntityAnimation* anim){
    setAnimation((Animation*)anim);
}

void EntityAnimationPlayer::setAnimation(const EntityAnimation* anim, double speed){
    setAnimation((Animation*)anim);
}