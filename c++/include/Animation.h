#pragma once
#include <memory>
#include "SDL2/SDL.h"
#include "Frame.h"
#include "util.h"

using namespace Kuribrawl;
struct Frame;

/**
 * @brief An animation (i.e.\ an ensemble of frames).
 * 
 * Animations are composed of a source image and an array of frames, which are just rectangle positions on the source image.
 */

class Animation {
    friend class CurrentAnimation;

    public:

    Animation();
    Animation(SDL_Texture* spritesheet);
    Animation(SDL_Texture* spritesheet, int nFrames);
    ~Animation();

    void setSpritesheet(SDL_Texture* spritesheet);
    SDL_Texture* getSpritesheet() const ;
    bool is_initialized();
    int getNbFrames();
    Frame* getFrame(int n); //Pointer validity : frames are stored in a unique pointer, can't be invalid as long as returns a frame of this animation
    void setBaseSpeed(double speed);

    void draw(SDL_Renderer* target, int x, int y, int frame);
    void draw(SDL_Renderer* target, int x, int y, int frame, int facing);

    //Construction
    void initFrames(int n);
    Hurtbox& addHurtbox();
    Hitbox& addHitbox();

    private:
    SDL_Texture* spritesheet; ///<SDL Texture used as the source image.
    SDL_Texture* spritesheet_left; ///< SDL Texture used as the alternative source image for entities that can be oriented in two different directions (typically Fighters)
    std::unique_ptr<Frame[]> frames; ///<Array (Basic array unique-pointed) containing the frames (= positions of the frames on the image) of this animation.
    int nb_frames; ///< Number of frames in this animation.
    Vector display; ///< Size of the source image.

    double base_speed;  /**< Speed of this animation. 
                        Can be < 1, in which case it will be used as a multiplier; or an integer, in which case it will be the total duration of the Animation.*/
};