#include "EntityAnimation.h"
#include "KBDebug/Debug.h"

using EndAction = AnimationEndAction<EntityAnimation>;

EntityAnimation::EntityAnimation():
    AnimationBase(),
    AnimationEndActionOwner<EntityAnimation>(EndAction::repeat_tag),
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
    AnimationBase::initFrames(n);

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

const EntityFrame& EntityAnimation::getEntityFrame(int n) const{
    if (n > nb_frames) {
        throw KBFatal("Frame index %d out of bounds", n);
    }
    return entity_frames[n];
}

const EndAction& EntityAnimation::getEndAction() const {
    return end_action;
}

/**
 * @brief DEPRECATED. Sets the \ref Animation#next "next animation".
 * The next animation is an Animation that will be started once this one finishes.
 */
void EntityAnimation::setNextAnimation(const EntityAnimation* anim){
    setEndAction(anim);
}

/**
 * @brief DEPRECATED.
 * 
 * @return const Animation* 
 */
const EntityAnimation* EntityAnimation::getNextAnimation() const {
    return (end_action.mode == EndAction::Mode::START_ANIMATION) ? end_action.data.next_anim : nullptr;
}

bool EntityAnimation::hasHitboxes() const {
    return has_hitboxes;
}

const std::vector<Hitbox>&  EntityAnimation::getHitboxes(int frame) const {
    if (frame > nb_frames) {
        throw KBFatal("Frame index %d out of bounds", frame);
    }
    return entity_frames[frame].hitboxes;
}

const std::vector<Hurtbox>& EntityAnimation::getHurtboxes(int frame) const {
    if (frame > nb_frames) {
        throw KBFatal("Frame index %d out of bounds", frame);
    }
    return entity_frames[frame].hurtboxes;
}

const EntityFrame::FrameMovement& EntityAnimation::getFrameMovement(int frame) const {
    if (frame > nb_frames) {
        throw KBFatal("Frame index %d out of bounds", frame);
    }
    return entity_frames[frame].movement; 
}

Hitbox* EntityAnimation::addHitbox(int frame){
    if (frame > nb_frames) {
        throw KBFatal("Frame index %d out of bounds", frame);
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
        throw KBFatal("Frame index %d out of bounds", frame);
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