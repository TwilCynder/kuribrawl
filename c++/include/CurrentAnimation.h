#pragma once
#include "SDL2/SDL.h"
#include "CBoxVectors.h"

class EntityAnimation;
class Fighter;

/**
 * @brief Object that can run an Animation.
 * Contains all the information needed for an Animation *while it is running*. A Current Animation object is not restrained to a specific Animation,
 * and can switch to another Animation completely at any time.
 */

class CurrentAnimation {
    public:

    CurrentAnimation();
    CurrentAnimation(EntityAnimation* animation);

    void draw(SDL_Renderer* target, int x, int y);
    void draw(SDL_Renderer* target, int x, int y, int facing);
    const std::vector<Hurtbox>& getHurtboxes() const;
    const std::vector<Hitbox>&  getHitboxes() const;

    bool is_initialized()const;
    bool is_finished();
    const EntityAnimation* getAnimation() const;
    void setAnimation(const EntityAnimation* anim);
    void setAnimation(const EntityAnimation* anim, double speed);
    void setSpeed(double speed);
    void advance();

    private:
    const EntityAnimation* model;   ///< The Animation that is running.
                                    /**< Pointer validity : can be invalidated if an Animation is deleted or moved (should not happend while a CurrentAnimation instance exists)*/
    
    int current_frame; ///< Index of the current frame.
    int timeleft; ///<Time remaining *on the current frame*.
    double speed; ///<Current \ref Animation#base_speed "speed" at which the animation runs (will override the base speed of the Animation).

    int frame_multiplier; ///< Time each frame will be displayed for.
    double base_carry; ///< If the total duration is not a multiple of the number of frames, certain frames will be displayed one frame-time longer than others. This calue is used to calculate which ones.
    double current_carry; ///< If the total duration is not a multiple of the number of frames, certain frames will be displayed one frame-time longer than others. This calue is used to calculate which ones.
    bool finished; ///< True if the animation just finished (even if it looped).
    Fighter* owner; ///< Entity running this animation. Currently restricted to \ref Fighter "Fighters".

    void nextFrame();
    void reset();
    void startFrame();
    void init();
    void start();
};