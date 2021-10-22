#include "EntityAnimation.h"
#include "Debug.h"

EntityAnimation::EntityAnimation():
    Animation(),
    entity_frames(nullptr),
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

void EntityAnimation::initFrames(int n){
    Animation::initFrames(n);

    entity_frames = std::make_unique<EntityFrame[]>(n);
}

/**
 * @brief Returns an entity frame of this EntityAnimation.
 * EntityFrames contain game-related informations about a frame (hitboxes, influence on movement)
 * @param n the id of the frame. Frame ids are just array indexes, thus start at 0.
 * @return Frame* a Frame. Pointer validity : frames are stored in a unique pointer, can't be invalid as long as the Animation exists.
 */

EntityFrame* EntityAnimation::getEntityFrame(int n){
    return &entity_frames[n];
}

void EntityAnimation::setNextAnimation(const EntityAnimation* anim){
    setNextAnimation((const Animation*)anim);
}

const EntityAnimation* EntityAnimation::getNextAnimation() const{
    return (const EntityAnimation*) Animation::getNextAnimation();
}

bool EntityAnimation::hasHitboxes() const {
    return has_hitboxes;
}

const std::vector<Hitbox>&  EntityAnimation::getHitboxes(int frame) const {
    if (frame > nb_frames) {
        throw KBFatal("Frame index out of bounds");
    }
    return entity_frames[frame].hitboxes;
}

const std::vector<Hurtbox>& EntityAnimation::getHurtboxes(int frame) const {
    if (frame > nb_frames) {
        throw KBFatal("Frame index out of bounds");
    }
    return entity_frames[frame].hurtboxes;
}

Hitbox* EntityAnimation::addHitbox(int frame){
    if (frame > nb_frames) {
        throw KBFatal("Frame index out of bounds");
    }
    return &entity_frames[frame].hitboxes.emplace_back();
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
    return &entity_frames[frame].hurtboxes.emplace_back();
}

void EntityAnimation::addHurtbox(int frame, int x, int y, int w, int h){
    Hurtbox* hb = addHurtbox(frame);
    hb->x = x;
    hb->y = y;
    hb->w = w;
    hb->h = h;
}