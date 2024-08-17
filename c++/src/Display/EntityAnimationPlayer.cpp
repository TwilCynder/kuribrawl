#include "KBDebug/Debug.h"
#include "Display/EntityAnimationPlayer.h"
#include "Display/EntityAnimation.h"
#include "Display/EntityAnimationPlayer.h"

#define model_ ((EntityAnimation*)model)

using EndAction = AnimationEndAction<EntityAnimation>;

EntityAnimationPlayer::EntityAnimationPlayer():
    AnimationPlayerBase<EntityAnimation, EntityAnimationStateManager>(),
    override_end_action(false)
{
}

/**
 * @brief Construct a new Current Animation object.
 *
 * @param animation the Animation that will be ran at first.
 */

EntityAnimationPlayer::EntityAnimationPlayer(const EntityAnimation* animation):
    AnimationPlayerBase<EntityAnimation, EntityAnimationStateManager>(animation),
    override_end_action(false)
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
    return model->getHurtboxes(current_frame);
}

const std::vector<Hitbox>& EntityAnimationPlayer::getHitboxes() const {
    return model->getHitboxes(current_frame);
}

const EntityFrame::FrameMovement& EntityAnimationPlayer::getFrameMovement() const {
    return model->getEntityFrame(current_frame).movement;
}

const GameplayAnimationBehavior::EndingBehavior EntityAnimationPlayer::getEndBehavior() const
{
    return model->gameplay_behavior.getEndBehavior();
}

const GameplayAnimationBehavior::LandingBehavior* EntityAnimationPlayer::getLandingBehavior() const
{
    if (this->is_initialized() && state_manager.currentLandingWindow != model->gameplay_behavior.getLandingBehavior().end()){
        return &state_manager.currentLandingWindow->behavior;
    } else {
        return nullptr;
    }
}

const EntityAnimation* EntityAnimationPlayer::getAnimation() const {
    return model;
}