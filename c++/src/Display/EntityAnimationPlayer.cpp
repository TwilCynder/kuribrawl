#include "KBDebug/Debug.h"
#include "Display/EntityAnimationPlayer.h"
#include "Display/EntityAnimation.h"
#include "Display/EntityAnimationPlayer.h"


EntityAnimationPlayer::EntityAnimationPlayer():
    AnimationPlayerBase<EntityAnimation, EntityAnimationStateManager>()
{
}

/**
 * @brief Construct a new Current Animation object.
 *
 * @param animation the Animation that will be ran at first.
 */

EntityAnimationPlayer::EntityAnimationPlayer(const EntityAnimation* animation):
    AnimationPlayerBase<EntityAnimation, EntityAnimationStateManager>(animation)
{}

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