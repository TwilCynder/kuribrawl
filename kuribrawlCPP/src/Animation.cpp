#include "Animation.h"
#include "KBException.h"
#include <stdio.h>

Animation::Animation():
    base_speed(1),
    nb_frames(0)
{

}

Animation::Animation(SDL_Texture* spritesheet){
    Animation();
    setSpritesheet(spritesheet);
    this->display.x = 0;
    this->display.y = 0;
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
    this->frames = std::make_unique<Frame[]>(n);
    int w = this->display.w / n;
    int h = this->display.h;
    int x = 0;
    Frame* frame;
    for (int i = 0; i < n; i++){
        frame = &this->frames[i];
        frame->display.x = x;
        frame->display.y = 0;
        frame->display.w = w;
        frame->display.h = h;
    }
}

void Animation::draw(SDL_Renderer* target, int x, int y){
    if (!this->spritesheet) return;
    SDL_RenderCopy(target, this->spritesheet, NULL, &this->display);
}