#include "Debug.h"
#include "Animation.h"
#include <stdio.h>

/**
 * @brief Construct a new Animation object, without any frames or spritesheet.
 * The frame array is not even created, which means that this object should not be used at all until frames are created with Animation::initFrames()
 */

Animation::Animation():
    base_speed(1),
    nb_frames(0),
    frames(NULL)
{
}

/**
 * @brief Construct a new Animation object, without any frames.
 * The frame array is not even created, which means that this object should not be used at all until frames are created with Animation::initFrames().
 * Delegates to Animation::Animation().
 * @param spritesheet_ SDL Texture that will be used as the source image, containing all frames.
 */

Animation::Animation(SDL_Texture* spritesheet_) : Animation(){
    setSpritesheet(spritesheet_);
}

/**
 * @brief Construct a new Animation object.
 * 
 * @param spritesheet SDL Texture that will be used as the source image, containing all frames.
 * @param nFrames Number of frames this animation has.
 */

Animation::Animation(SDL_Texture* spritesheet, int nFrames) : Animation(spritesheet){
    initFrames(nFrames);
}

/**
 * @brief Destroy the Animation object
 * 
 */

Animation::~Animation(){
}

/**
 * @brief Sets the current spritesheeet (source image)
 * 
 * @param spritesheet the SDL texture that will be used as the spritesheet.
 */

void Animation::setSpritesheet(SDL_Texture* spritesheet){
    if (!spritesheet){
        throw KBFatal("Animation constructor : spritesheet texture pointer is null");
    }
    this->spritesheet = spritesheet;
    SDL_QueryTexture(spritesheet, NULL, NULL, &this->display.x, &this->display.y);
}

/**
 * @brief Returns the current source image
 * 
 * @return SDL_Texture* 
 */

SDL_Texture* Animation::getSpritesheet() const {
    return spritesheet;
}

/**
 * @brief Initializes this Animation's frames.
 * This function creates the frames array (until it's called, the array is a NULL pointer). 
 * 
 * @param n 
 */

void Animation::initFrames(int n){
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

bool Animation::is_initialized(){
    return spritesheet;
}

int Animation::getNbFrames(){
    return nb_frames;
}

Frame* Animation::getFrame(int n){
    return &frames[n];
}

void Animation::setBaseSpeed(double speed){
    base_speed = speed;
}

void Animation::draw(SDL_Renderer* target, int x, int y, int frame){
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