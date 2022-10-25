#include "EntityAnimation.h"
#include "KBDebug/Debug.h"

EntityAnimation::EndActionData::EndActionData(const EntityAnimation* anim) : next_anim(anim) {}
EntityAnimation::EndActionData::EndActionData(int code_) : code(code_) {}
EntityAnimation::EndActionData::EndActionData() : next_anim(nullptr) {}

EntityAnimation::EntityAnimation():
    Animation(),
    entity_frames(nullptr),
    has_hitboxes(false),
    end_action_mode(EndAction::REPEAT)
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

const EntityFrame& EntityAnimation::getEntityFrame(int n) const{
    if (n > nb_frames) {
        throw KBFatal("Frame index %d out of bounds", n);
    }
    return entity_frames[n];
}

void EntityAnimation::setEndAction(EntityAnimation::EndAction mode, EntityAnimation::EndActionData action){
    end_action_mode = mode;
    end_action = action;
}

void EntityAnimation::setEndAction(){
    end_action_mode = EndAction::REPEAT;
    loop = true;
}

void EntityAnimation::setEndAction(int code_){
    end_action_mode = EndAction::RETURN_CODE;
    end_action = code_;
    loop = false;
}

void EntityAnimation::setEndAction(const EntityAnimation* anim){
    end_action_mode = EndAction::START_ANIMATION;
    end_action = anim;
    loop = false;
}

EntityAnimation::EndAction EntityAnimation::getEndActionMode() const{
    return end_action_mode;
}

const EntityAnimation::EndActionData& EntityAnimation::getEndAction() const{
    return end_action;
}

/**
 * @brief DEPRECATED. Sets the \ref Animation#next "next animation".
 * The next animation is an Animation that will be started once this one finishes.
 */
void EntityAnimation::setNextAnimation(const EntityAnimation* anim){
    end_action_mode = EndAction::START_ANIMATION;
    end_action.next_anim = anim;
}

/**
 * @brief DEPRECATED.
 * 
 * @return const Animation* 
 */
const EntityAnimation* EntityAnimation::getNextAnimation() const {
    return (end_action_mode == EndAction::START_ANIMATION) ? end_action.next_anim : nullptr;
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