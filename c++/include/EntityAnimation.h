#pragma once
#include "Animation.h"
#include "EntityFrame.h"
#include "CBoxVectors.h"

class EntityAnimation : public Animation {
    public:
    EntityAnimation();
    EntityAnimation(SDL_Texture* spritesheet);
    EntityAnimation(SDL_Texture* spritesheet, int nFrames);
    ~EntityAnimation();

    void initFrames(int n);
    EntityFrame* getEntityFrame (int n);
    void setNextAnimation(const EntityAnimation*);
    const EntityAnimation* getNextAnimation() const;

    bool hasHitboxes() const;
    const std::vector<Hitbox>&  getHitboxes (int frame) const;
    const std::vector<Hurtbox>& getHurtboxes(int frame) const;
    Hitbox*  addHitbox (int frame);
    void addHitbox(int frame, int, int, int, int);
    Hurtbox* addHurtbox(int frame);
    void addHurtbox(int frame, int, int, int, int);

    private:
    using Animation::setNextAnimation;
    std::unique_ptr<EntityFrame[]> entity_frames;
    bool has_hitboxes;
};
