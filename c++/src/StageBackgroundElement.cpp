#include "StageBackgroundElement.h"
#include "StageBackgroundElementModel.h"
#include "defs.h"

#include "KBDebug/Debug.h"

StageBackgroundElement::StageBackgroundElement(const StageBackgroundElementModel& model_):
    model(model_), animation_player(&model_.animation)
{}

void StageBackgroundElement::advanceAnimation(){
    animation_player.advance();
}

void StageBackgroundElement::draw(SDL_Renderer* target) const {
    animation_player.draw(target, model.position.x, SCREEN_HEIGHT - model.position.y);
}

const StageBackgroundElementModel& StageBackgroundElement::getModel() const {
    return model;
}