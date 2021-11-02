#include "EntityAnimation.h"
#include "Debug.h"

EntityAnimation::EntityAnimation():
    Animation(),
    has_hitboxes(false)
{
}

EntityAnimation::EntityAnimation(SDL_Texture* spritesheet_) : EntityAnimation(){
    setSpritesheet(spritesheet_);
}

EntityAnimation::EntityAnimation(SDL_Texture* spritesheet, int nFrames) : EntityAnimation(spritesheet){
    initFrames(nFrames);
}

EntityAnimation::~EntityAnimation(){
}

bool EntityAnimation::hasHitboxes() const {
    return has_hitboxes;
}

const std::vector<Hitbox>&  EntityAnimation::getHitboxes(int frame) const {
    if (frame > nb_frames) {
        throw KBFatal("Frame index out of bounds");
    }
    return frames[frame].hitboxes;
}

const std::vector<Hurtbox>& EntityAnimation::getHurtboxes(int frame) const {
    if (frame > nb_frames) {
        throw KBFatal("Frame index out of bounds");
    }
    return frames[frame].hurtboxes;
}

const EntityFrame::FrameMovement& EntityAnimation::getFrameMovement(int frame) const {
}

Hitbox* EntityAnimation::addHitbox(int frame){
    if (frame > nb_frames) {
        throw KBFatal("Frame index out of bounds");
    }
    return &frames[frame].hitboxes.emplace_back();
}

void EntityAnimation::addHitbox(int frame, int x, int y, int w, int h){
    Hitbox* hb = addHitbox(frame);
    hb->x = x;
    hb->y = y;
    hb->w = w;
    hb->h = h;

    has_hitboxes = true;
}

Hurtbox* EntityAnimation::addHurtbox(int frame){
    if (frame > nb_frames) {
        throw KBFatal("Frame index out of bounds");
    }
    return &frames[frame].hurtboxes.emplace_back();
}

void EntityAnimation::addHurtbox(int frame, int x, int y, int w, int h){
    Hurtbox* hb = addHurtbox(frame);
    hb->x = x;
    hb->y = y;
    hb->w = w;
    hb->h = h;
}