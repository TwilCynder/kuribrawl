#include "KBDebug/Debug.h"
#include "Animation/EntityAnimationPlayer.h"
#include "Animation/EntityAnimation.h"

#define model_ ((EntityAnimation*)model)

using EndAction = AnimationEndAction<EntityAnimation>;

EntityAnimationPlayer::EntityAnimationPlayer():
    AnimationPlayerBase(),
    override_end_action(false)
{
}

/**
 * @brief Construct a new Current Animation object.
 *
 * @param animation the Animation that will be ran at first.
 */

EntityAnimationPlayer::EntityAnimationPlayer(EntityAnimation* animation):
    AnimationPlayerBase((Animation*)animation)
{
}

const EndAction& EntityAnimationPlayer::resolveEndAction() const {
    return (override_end_action) ? end_action : model_->getEndAction();
};


int EntityAnimationPlayer::advance(){
    const EndAction& current_end_action = resolveEndAction();

    AnimationPlayerBase::advance(current_end_action.mode == EndAction::Mode::REPEAT);

    if (finished){
        override_end_action = false;
        switch (current_end_action.mode){
        case EndAction::Mode::REPEAT :
        case EndAction::Mode::NONE   :
            break;
        case EndAction::Mode::RETURN_CODE:
            return current_end_action.data.code;
        case EndAction::Mode::START_ANIMATION:
        {
            const EntityAnimation* next;
            next = current_end_action.data.next_anim;  
            if (next != nullptr){
                setAnimation(next);
            }
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
    setAnimation((Animation*)anim, speed);
}