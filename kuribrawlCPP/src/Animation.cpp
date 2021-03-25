#include "Debug.h"
#include "Animation.h"
#include <stdio.h>

Animation::Animation():
    base_speed(1),
    nb_frames(0)
{
}

Animation::Animation(SDL_Texture* spritesheet) : Animation(){
    setSpritesheet(spritesheet);
    this->display.x = 0;
    this->display.y = 0;
}

Animation::Animation(SDL_Texture* spritesheet, int nFrames) : Animation(spritesheet){
    initFrames(nFrames);
}

Animation::~Animation(){

}

void Animation::setSpritesheet(SDL_Texture* spritesheet){
    if (!spritesheet){
        throw KBFatal("Animation constructor : spritesheet texture pointer is null");
    }
    this->spritesheet = spritesheet;
    SDL_QueryTexture(spritesheet, NULL, NULL, &this->display.w, &this->display.h);
}

void Animation::initFrames(int n){
    if (n < 1){
        throw KBFatal("Tried to init animation with 0 frames");
    }

    frames = std::make_unique<Frame[]>(n);
    int w = display.w / n;
    int h = display.h;
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

Animation::Frame* Animation::getFrame(int n){
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