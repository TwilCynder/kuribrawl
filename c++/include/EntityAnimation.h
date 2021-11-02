#pragma once
#include "Animation.h"
#include "EntityFrame.h"
#include "CBoxVectors.h"

class EntityAnimation : public Animation<EntityFrame> {
    public:
    EntityAnimation();
    EntityAnimation(SDL_Texture* spritesheet);
    EntityAnimation(SDL_Texture* spritesheet, int nFrames);
    ~EntityAnimation();

    bool hasHitboxes() const;
    const std::vector<Hitbox>&  getHitboxes (int frame) const;
    const std::vector<Hurtbox>& getHurtboxes(int frame) const;
    const EntityFrame::FrameMovement& getFrameMovement(int frame) const;
    Hitbox*  addHitbox (int frame);
    void addHitbox(int frame, int, int, int, int);
    Hurtbox* addHurtbox(int frame);
    void addHurtbox(int frame, int, int, int, int);

    private:
    bool has_hitboxes;
};
