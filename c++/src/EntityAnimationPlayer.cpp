#include "Debug.h"
#include "EntityAnimationPlayer.h"
#include "EntityAnimation.h"

#define model_ ((EntityAnimation*)model)

EntityAnimationPlayer::EntityAnimationPlayer():
    AnimationPlayer()
{
}

/**
 * @brief Construct a new Current Animation object.
 *
 * @param animation the Animation that will be ran at first.
 */

EntityAnimationPlayer::EntityAnimationPlayer(EntityAnimation* animation):
    AnimationPlayer((Animation*)animation)
{
}

int EntityAnimationPlayer::advance(){
    AnimationPlayer::advance();
    if (finished){
        const EntityAnimation* next;
        switch (model_->getEndActionMode()){
            case EntityAnimation::EndAction::REPEAT:
                break;
            case EntityAnimation::EndAction::RETURN_CODE:
                return model_->getEndAction().code;
            case EntityAnimation::EndAction::START_ANIMATION:
                next = model_->getEndAction().next_anim;
                if (next != nullptr){
                    setAnimation(next);
                }
                break;
        }
    }
    return 0;
}

const std::vector<Hurtbox>& EntityAnimationPlayer::getHurtboxes() const {
    return model_->getHurtboxes(current_frame);
}

const std::vector<Hitbox>& EntityAnimationPlayer::getHitboxes() const {
    return model_->getHitboxes(current_frame);
}

const EntityFrame::FrameMovement& EntityAnimationPlayer::getFrameMovement() const {
    return model_->getEntityFrame(current_frame)->movement;
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