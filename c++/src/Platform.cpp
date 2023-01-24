#include "PlatformModel.h"
#include "Platform.h"
#include "Camera.h"

Platform::Platform(const PlatformModel& model_) : 
    model(model_)
{
    const Animation* model_animation = model.animation;
    if (model_animation){
        animation_player = std::make_unique<AnimationPlayer>(model_animation);
    }
}

bool Platform::hasAnimation() const {
    return animation_player.get() != nullptr;
}

void Platform::advanceAnimation(){
    if (!animation_player.get()) return;
    animation_player->advance();
}

void Platform::draw(SDL_Renderer* target, const Camera& cam) const {
    if (!animation_player.get()) return;
    animation_player->draw(target, cam.getXOnScreen(model.pos.x), cam.getYOnScreen(model.pos.y));
}