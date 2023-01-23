#include "Stage.h"
#include "StageModel.h"
#include "Game.h"

#include "KBDebug/Debug.h"

Stage::Stage(const StageModel& m) :
    model(m)
{
    const std::vector<StageBackgroundElementModel>& background_element_models = m.getBackgroundElements();
    background_elements.reserve(background_element_models.size());
    for (const StageBackgroundElementModel& background_element_model : background_element_models){
        background_elements.emplace_back(background_element_model);
    }

    Debug::log(background_elements.size());
}

const StageModel& Stage::getModel() const {
    return model;
}

const std::vector<StageBackgroundElement> Stage::getBackgroundElements() const {
    return background_elements;
}