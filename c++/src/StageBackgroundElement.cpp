#include "StageBackgroundElement.h"
#include "StageBackgroundElementModel.h"
#include "defs.h"
#include "Camera.h"

#include "KBDebug/Debug.h"

StageBackgroundElement::StageBackgroundElement(const StageBackgroundElementModel& model_):
    model(model_), animation_player(&model_.animation)
{}

void StageBackgroundElement::advanceAnimation(){
    animation_player.advance();
}

void StageBackgroundElement::draw(SDL_Renderer* target, const Camera& cam) const {
    animation_player.draw(target, cam.getXOnScreen(model.position.x), cam.getYOnScreen(model.position.y));
}

const StageBackgroundElementModel& StageBackgroundElement::getModel() const {
    return model;
}