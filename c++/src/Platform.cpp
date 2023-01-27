#include "PlatformModel.h"
#include "Platform.h"
#include "Camera.h"

Platform::Platform(const PlatformModel& model_) : 
    model(model_), position(model_.pos)
{
    const Animation* model_animation = model.animation;
    if (model_animation){
        animation_player = std::make_unique<AnimationPlayer>(model_animation);
    }
}

bool Platform::hasAnimation() const {
    return animation_player.get() != nullptr;
}

const Kuribrawl::Vector &Platform::getPosition() const
{
    return position;
}

const int Platform::getWidth() const {
    return model.half_w * 2 + 1;
}

const int Platform::getHalfWidth() const {
    return model.half_w;
}

void Platform::advanceAnimation(){
    if (!animation_player.get()) return;
    animation_player->advance();
}

void Platform::draw(SDL_Renderer* target, const Camera& cam) const {
    if (!animation_player.get()) return;
    animation_player->draw(target, cam.getXOnScreen(model.pos.x), cam.getYOnScreen(model.pos.y));
    SDL_SetRenderDrawColor(target, 255, 255, 0, 255);
    //SDL_RenderDrawLine(target, position.x - getWidth() / 2);
}