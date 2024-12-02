#include "Display/EntityAnimation.h"
#include "KBDebug/Debug.h"


EntityAnimation::EntityAnimation():
    AnimationBaseWithEndAction<EntityAnimation>(),
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

void EntityAnimation::initFrames(frame_index_t n){
    AnimationBase::initFrames(n);

    entity_frames = std::make_unique<EntityFrame[]>(n);
}

/**
 * @brief Returns an entity frame of this EntityAnimation.
 * EntityFrames contain game-related informations about a frame (hitboxes, influence on movement)
 * @param n the id of the frame. Frame ids are just array indexes, thus start at 0.
 * @return Frame* a Frame. Pointer validity : frames are stored in a unique pointer, can't be invalid as long as the Animation exists.
 */

EntityFrame* EntityAnimation::getEntityFrame(frame_index_t n){
    return &entity_frames[n];
}

const EntityFrame& EntityAnimation::getEntityFrame(frame_index_t n) const{
    if (n > nb_frames) {
        throw KBFatal("Frame index %d out of bounds", n);
    }
    return entity_frames[n];
}

bool EntityAnimation::hasHitboxes() const {
    return has_hitboxes;
}

const std::vector<Hitbox>&  EntityAnimation::getHitboxes(frame_index_t frame) const {
    if (frame > nb_frames) {
        throw KBFatal("Frame index %d out of bounds", frame);
    }
    return entity_frames[frame].hitboxes;
}

const std::vector<Hurtbox>& EntityAnimation::getHurtboxes(frame_index_t frame) const {
    if (frame > nb_frames) {
        throw KBFatal("Frame index %d out of bounds", frame);
    }
    return entity_frames[frame].hurtboxes;
}

const EntityFrame::FrameMovement& EntityAnimation::getFrameMovement(frame_index_t frame) const {
    if (frame > nb_frames) {
        throw KBFatal("Frame index %d out of bounds", frame);
    }
    return entity_frames[frame].movement; 
}

Hitbox* EntityAnimation::addHitbox(frame_index_t frame){
    if (frame > nb_frames) {
        throw KBFatal("Frame index %d out of bounds", frame);
    }
    return &entity_frames[frame].hitboxes.emplace_back();
}

void EntityAnimation::addHitbox(frame_index_t frame, int x, int y, int w, int h){
    Hitbox* hb = addHitbox(frame);
    hb->x = x;
    hb->y = y;
    hb->w = w;
    hb->h = h;

    has_hitboxes = true;
}

Hurtbox* EntityAnimation::addHurtbox(frame_index_t frame){
    if (frame > nb_frames) {
        throw KBFatal("Frame index %d out of bounds", frame);
    }
    return &entity_frames[frame].hurtboxes.emplace_back();
}

void EntityAnimation::addHurtbox(frame_index_t frame, int x, int y, int w, int h){
    Hurtbox* hb = addHurtbox(frame);
    hb->x = x;
    hb->y = y;
    hb->w = w;
    hb->h = h;
}
