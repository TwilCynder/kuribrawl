#include "Debug.h"
#include "Animation.h"
#include <stdio.h>

/**
 * @brief Construct a new Animation object, without any frames or spritesheet.
 * The frame array is not even created, which means that this object should not be used at all until frames are created with Animation::initFrames()
 */

template<class FrameType>
Animation<FrameType>::Animation():
    nb_frames(0),
    frames(NULL),
    base_speed(1),
    end_action{NULL}
{
}

/**
 * @brief Construct a new Animation object, without any frames.
 * The frame array is not even created, which means that this object should not be used at all until frames are created with Animation::initFrames().
 * Delegates to Animation::Animation().
 * @param spritesheet_ SDL Texture that will be used as the source image, containing all frames.
 */
template<class FrameType>
Animation<FrameType>::Animation(SDL_Texture* spritesheet_) : Animation(){
    setSpritesheet(spritesheet_);
}

/**
 * @brief Construct a new Animation object.
 *
 * @param spritesheet SDL Texture that will be used as the source image, containing all frames.
 * @param nFrames Number of frames this animation has.
 */
template<class FrameType>
Animation<FrameType>::Animation(SDL_Texture* spritesheet, int nFrames) : Animation(spritesheet){
    initFrames(nFrames);
}

/**
 * @brief Destroy the Animation object
 *
 */
template<class FrameType>
Animation<FrameType>::~Animation(){
}

/**
 * @brief Sets the current spritesheeet (source image)
 *
 * @param spritesheet the SDL texture that will be used as the spritesheet.
 */
template<class FrameType>
void Animation<FrameType>::setSpritesheet(SDL_Texture* spritesheet_){
    if (!spritesheet_){
        throw KBFatal("SetSpritesheet : spritesheet texture pointer is null");
    }
    this->spritesheet = spritesheet_;
    SDL_QueryTexture(spritesheet, NULL, NULL, &this->display.x, &this->display.y);
}

/**
 * @brief Returns the current source image
 *
 * @return SDL_Texture*
 */
template<class FrameType>
SDL_Texture* Animation<FrameType>::getSpritesheet() const {
    return spritesheet;
}

/**
 * @brief Initializes this Animation's frames.
 * This function creates the frames array (until it's called, the array is a NULL pointer).
 * The frames positions are initialized considering that the image contains all the frame lined up horizontally,
 * filling the entire image.
 * The origin of each frame is (width/2 ; height)
 * @param n
 */
template<class FrameType>
void Animation<FrameType>::initFrames(int n){
    if (n < 1){
        throw KBFatal("Tried to init animation with 0 frames");
    }

    frames = std::make_unique<Frame[]>(n);
    int w = display.x / n;
    int h = display.y;
    int x = 0;
    Frame* frame;
    for (int i = 0; i < n; i++){
        frame = &frames[i];
        frame->display.x = x;
        frame->display.y = 0;
        frame->display.w = w;
        frame->display.h = h;
        frame->origin.x = w / 2;
        frame->origin.y = h;
        x += w;
    }
    nb_frames = n;
}
template<class FrameType>
void Animation<FrameType>::setEndAction(Animation::EndAction mode, Animation::EndActionData action){
    end_action_mode = mode;
    end_action = action;
}

/**
 * @brief DEPRECATED. Sets the \ref Animation#next "next animation".
 * The next animation is an Animation that will be started once this one finishes.
 */
template<class FrameType>
void Animation<FrameType>::setNextAnimation(const Animation<FrameType>* anim){
    end_action_mode = EndAction::START_ANIMATION;
    end_action.next_anim = anim;
}

/**
 * @brief DEPRECATED.
 * 
 * @return const Animation* 
 */
template<class FrameType>
const Animation<FrameType>* Animation<FrameType>::getNextAnimation() const {
    return (end_action_mode == EndAction::START_ANIMATION) ? end_action.next_anim : nullptr;
}

/**
 * @brief Returns whether this Animation is ready for use
 *
 * @return true if this Animation has a valid spritesheet and frames array.
 * @return false otherwise.
 */
template<class FrameType>
bool Animation<FrameType>::is_initialized() const {
    return spritesheet && frames;
}

/**
 * @brief Returns the number of frames this Animation has.
 *
 * @return int
 */
template<class FrameType>
int Animation<FrameType>::getNbFrames(){
    return nb_frames;
}

/**
 * @brief Returns a frame of this Animation.
 *
 * @param n the id of the frame. Frame ids are just array indexes, thus start at 0.
 * @return Frame* a Frame. Pointer validity : frames are stored in a unique pointer, can't be invalid as long as the Animation exists.
 */
template<class FrameType>
FrameType* Animation<FrameType>::getFrame(int n){
    return &frames[n];
}

/**
 * @brief Sets the base speed of this animtion.
 * Can be < 1, in which case it will be used as a multiplier, or an integer, in which case it will
 * be the total duration of the Animation.
 * @param speed
 */
template<class FrameType>
void Animation<FrameType>::setBaseSpeed(double speed){
    base_speed = speed;
}

/**
 * @brief Returns the base speed of this animtion.
 * Can be < 1, in which case it will be used as a multiplier, or an integer, in which case it will
 * be the total duration of the Animation.
 * Any value that isn't either of those (a non-integer > 1) is invalid and will cause **undefined behavior**
 * @return the speed
 */
template<class FrameType>
double Animation<FrameType>::getBaseSpeed(){
    return base_speed;
}


/**
 * @brief Draws a certain frame of this Animation.
 * Uses SDL_RenderCopy to copy the rectangle designated frame from.
 * @param target the renderer the frame will be drawn to.
 * @param x x position in the destination.
 * @param y y position in the destination.
 * @param frame index of the frame.
 */
template<class FrameType>
void Animation<FrameType>::draw(SDL_Renderer* target, int x, int y, int frame)const{
    if (!this->spritesheet) return;

    if (frame > nb_frames) {
        throw KBFatal("Frame index out of bounds");
    }

    Frame& source = frames[frame];

    SDL_Rect dest;
    dest.x = x - source.origin.x;
    dest.y = y - source.origin.y;
    dest.w = source.display.w;
    dest.h = source.display.h;

    SDL_RenderCopy(target, spritesheet, &source.display, &dest);
}

template<class FrameType>
void Animation<FrameType>::draw(SDL_Renderer* target, int x, int y, int frame, int facing)const{
    if (!this->spritesheet) return;

    if (frame > nb_frames) {
        throw KBFatal("Frame index out of bounds");
    }

    Frame& source = (Frame&)frames[frame];

    SDL_Rect dest;
    dest.x = x - source.origin.x;
    dest.y = y - source.origin.y;
    dest.w = source.display.w;
    dest.h = source.display.h;

    if (facing == -1)
        SDL_RenderCopyEx(target, spritesheet, &source.display, &dest, 0, NULL, SDL_FLIP_HORIZONTAL);
    else
        SDL_RenderCopy(target, spritesheet, &source.display, &dest);
}