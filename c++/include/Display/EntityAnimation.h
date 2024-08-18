#pragma once
#include "Animation.h"
#include "EntityFrame.h"
#include "CBoxVectors.h"
#include "AnimationEndAction.h"
#include "Types/Data.h"
#include "GameplayAnimationBehavior.h"

using namespace Kuribrawl::Types;

class EntityAnimation : public AnimationBase, public AnimationEndActionOwner<EntityAnimation> {
    public:

    using LandingBehavior = GameplayAnimationBehavior::LandingBehavior;
    using LandingBehaviorType = GameplayAnimationBehavior::LandingBehaviorType;
    using EndingBehavior = GameplayAnimationBehavior::EndingBehavior;

    EntityAnimation();
    EntityAnimation(SDL_Texture* spritesheet);
    EntityAnimation(SDL_Texture* spritesheet, int nFrames);
    ~EntityAnimation();

    void initFrames(frame_index_t n);
    EntityFrame* getEntityFrame (frame_index_t);
    const EntityFrame& getEntityFrame(frame_index_t) const;

    bool hasHitboxes() const;
    const std::vector<Hitbox>&  getHitboxes (frame_index_t frame) const;
    const std::vector<Hurtbox>& getHurtboxes(frame_index_t frame) const;
    const EntityFrame::FrameMovement& getFrameMovement(frame_index_t frame) const;
    Hitbox*  addHitbox (frame_index_t frame);
    void addHitbox(frame_index_t frame, int, int, int, int);
    Hurtbox* addHurtbox(frame_index_t frame);
    void addHurtbox(frame_index_t frame, int, int, int, int);

    
    template <typename... Args> requires EntityAnimationEndActionValidArg<EntityAnimation, Args...>
    void setEndAction(Args... args){
        end_action.set(args...);
    }

    const EndAction& getEndAction() const;
    void setNextAnimation(const EntityAnimation*);
    const EntityAnimation* getNextAnimation() const;

    GameplayAnimationBehavior gameplay_behavior;
    private:
    std::unique_ptr<EntityFrame[]> entity_frames;
    bool has_hitboxes;
};
