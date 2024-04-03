#include "KBDebug/Debug.h"
#include "Util/std_util.h"
#include "Display/Animation.h"
#include <stdio.h>

/**
 * @brief Construct a new AnimationBase object, without any frames or spritesheet.
 * The frame array is not even created, which means that this object should not be used at all until frames are created with AnimationBase::initFrames()
 */

AnimationBase::AnimationBase():
    nb_frames(0),
    loop(true),
    spritesheet(nullptr),
    frames(nullptr),
    base_speed(1)
{
}

/**
 * @brief Construct a new AnimationBase object, without any frames.
 * The frame array is not even created, which means that this object should not be used at all until frames are created with AnimationBase::initFrames().
 * Delegates to AnimationBase::AnimationBase().
 * @param spritesheet_ SDL Texture that will be used as the source image, containing all frames.
 */

AnimationBase::AnimationBase(SDL_Texture* spritesheet_) : AnimationBase(){
    setSpritesheet(spritesheet_);
}

/**
 * @brief Construct a new AnimationBase object.
 *
 * @param spritesheet SDL Texture that will be used as the source image, containing all frames.
 * @param nFrames Number of frames this AnimationBase has.
 */

AnimationBase::AnimationBase(SDL_Texture* spritesheet, int nFrames) : AnimationBase(spritesheet){
    initFrames(nFrames);
}

/**
 * @brief Destroy the AnimationBase object
 *
 */

AnimationBase::~AnimationBase(){
}

/**
 * @brief Sets the current spritesheeet (source image)
 *
 * @param spritesheet the SDL texture that will be used as the spritesheet.
 */

void AnimationBase::setSpritesheet(SDL_Texture* spritesheet_){
    if (!spritesheet_){
        throw KBFatal(Kuribrawl::formatString("SetSpritesheet : spritesheet texture pointer is null"));
    }
    this->spritesheet = spritesheet_;
    SDL_QueryTexture(spritesheet, NULL, NULL, &this->display.x, &this->display.y);
}

/**
 * @brief Returns the current source image
 *
 * @return SDL_Texture*
 */

SDL_Texture* AnimationBase::getSpritesheet() const {
    return spritesheet;
}

/**
 * @brief Initializes this AnimationBase's frames.
 * This function creates the frames array (until it's called, the array is a NULL pointer).
 * The frames positions are initialized considering that the image contains all the frame lined up horizontally,
 * filling the entire image.
 * The origin of each frame is (width/2 ; height)
 * @param n
 */

void AnimationBase::initFrames(int n){
    if (n < 1){
        throw KBFatal("Tried to init AnimationBase with 0 frames");
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

/**
 * @brief Returns whether this AnimationBase is ready for use
 *
 * @return true if this AnimationBase has a valid spritesheet and frames array.
 * @return false otherwise.
 */

bool AnimationBase::is_initialized() const {
    return spritesheet && frames;
}

/**
 * @brief Returns the number of frames this AnimationBase has.
 *
 * @return int
 */

int AnimationBase::getNbFrames(){
    return nb_frames;
}

/**
 * @brief Returns a frame of this AnimationBase.
 *
 * @param n the id of the frame. Frame ids are just array indexes, thus start at 0.
 * @return Frame* a Frame. Pointer validity : frames are stored in a unique pointer, can't be invalid as long as the AnimationBase exists.
 */

Frame* AnimationBase::getFrame(int n){
    return &frames[n];
}

/**
 * @brief Sets the base speed of this animtion.
 * Can be < 1, in which case it will be used as a multiplier, or an integer, in which case it will
 * be the total duration of the AnimationBase.
 * @param speed
 */

void AnimationBase::setBaseSpeed(double speed){
    base_speed = speed;
}

/**
 * @brief Returns the base speed of this animtion.
 * Can be < 1, in which case it will be used as a multiplier, or an integer, in which case it will
 * be the total duration of the AnimationBase.
 * Any value that isn't either of those (a non-integer > 1) is invalid and will cause **undefined behavior**
 * @return the speed
 */

double AnimationBase::getBaseSpeed(){
    return base_speed;
}


/**
 * @brief Draws a certain frame of this AnimationBase.
 * Uses SDL_RenderCopy to copy the rectangle designated frame from.
 * @param target the renderer the frame will be drawn to.
 * @param x x position in the destination.
 * @param y y position in the destination.
 * @param frame index of the frame.
 */

void AnimationBase::draw(SDL_Renderer* target, int x, int y, int frame)const{
    return draw(target, x, y, frame, Kuribrawl::Side::RIGHT);
}

/**
 * @brief Draws a specific frame of this AnimationBase. 
 * Uses SDL_RenderCopy to copy the rectangle designated frame from.
 * @param target the renderer the frame will be drawn to.
 * @param x x position in the destination.
 * @param y y position in the destination.
 * @param frame index of the frame.
 * @param facing orientation of the frame
 */
void AnimationBase::draw(SDL_Renderer* target, int x, int y, int frame, Kuribrawl::Side facing)const{
    if (!this->spritesheet) return;

    if (frame > nb_frames) {
        throw KBFatal("Frame index %d out of bounds", frame);
    }

    Frame& source = frames[frame];

    SDL_Rect dest;
    dest.x = x + (facing == Kuribrawl::Side::RIGHT ? -source.origin.x : source.origin.x - source.display.w);
    dest.y = y - source.origin.y;
    dest.w = source.display.w;
    dest.h = source.display.h;

    if (facing == Kuribrawl::Side::LEFT)
        SDL_RenderCopyEx(target, spritesheet, &source.display, &dest, 0, NULL, SDL_FLIP_HORIZONTAL);
    else
        SDL_RenderCopy(target, spritesheet, &source.display, &dest);
}

bool AnimationBase::operator==(const AnimationBase &other) const
{
    return 
        spritesheet == other.spritesheet && 
        nb_frames == other.nb_frames &&
        base_speed == other.base_speed;
}

std::weak_ordering AnimationBase::operator<=>(const AnimationBase &other) const
{
    std::weak_ordering order = spritesheet <=> other.spritesheet;
    return 
        (order != 0) ? order : 
        ((order = (nb_frames <=> other.nb_frames)) != 0) ? order : 
        Kuribrawl::cmp(base_speed, other.base_speed);
    
}