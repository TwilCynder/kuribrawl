#pragma once
#include "Animation.h"
#include "EntityFrame.h"
#include "CBoxVectors.h"
#include "AnimationEndAction.h"

class EntityAnimation : public AnimationBase, public AnimationEndActionOwner<EntityAnimation> {
    public:

    EntityAnimation();
    EntityAnimation(SDL_Texture* spritesheet);
    EntityAnimation(SDL_Texture* spritesheet, int nFrames);
    ~EntityAnimation();

    void initFrames(int n);
    EntityFrame* getEntityFrame (int n);
    const EntityFrame& getEntityFrame(int n) const;

    bool hasHitboxes() const;
    const std::vector<Hitbox>&  getHitboxes (int frame) const;
    const std::vector<Hurtbox>& getHurtboxes(int frame) const;
    const EntityFrame::FrameMovement& getFrameMovement(int frame) const;
    Hitbox*  addHitbox (int frame);
    void addHitbox(int frame, int, int, int, int);
    Hurtbox* addHurtbox(int frame);
    void addHurtbox(int frame, int, int, int, int);

    
    template <typename... Args> requires EntityAnimationEndActionValidArg<EntityAnimation, Args...>
    void setEndAction(Args... args){
        end_action.set(args...);
    }

    const EndAction& getEndAction() const;
    void setNextAnimation(const EntityAnimation*);
    const EntityAnimation* getNextAnimation() const;

    private:
    std::unique_ptr<EntityFrame[]> entity_frames;
    bool has_hitboxes;
};
