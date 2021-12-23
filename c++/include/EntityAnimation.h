#pragma once
#include "Animation.h"
#include "EntityFrame.h"
#include "CBoxVectors.h"

class EntityAnimation : public Animation {
    public:
    enum class EndAction {
        REPEAT,
        START_ANIMATION,
        RETURN_CODE
    };;

    union EndActionData {
        const EntityAnimation* next_anim; /**< Animation that should be started by the AnimationPlayer when this one finishes.
                            Poiter Validity : can be invalidated if the target animation if moved or destroyed (normal behavior should not make this happen)*/
        int code;   ///< A code to be passed to the caller of advance()
        EndActionData(const EntityAnimation*);
        EndActionData(int);
        EndActionData();
    };

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

    void setEndAction(EndAction mode, EndActionData action);
    void setEndAction();
    void setEndAction(const EntityAnimation* anim);
    void setEndAction(int code);
    EndAction getEndActionMode() const;
    const EndActionData& getEndAction() const;
    void setNextAnimation(const EntityAnimation*);
    const EntityAnimation* getNextAnimation() const;

    private:
    std::unique_ptr<EntityFrame[]> entity_frames;
    bool has_hitboxes;

    
    EndAction end_action_mode;
    EndActionData end_action;
};
