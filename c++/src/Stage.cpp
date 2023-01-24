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

    const std::vector<PlatformModel>& platform_models = m.getPlatforms();
    platforms.reserve(platform_models.size());
    for (const PlatformModel& platform_model: platform_models){
        platforms.emplace_back(platform_model);
    }

    Debug::log(platforms.size());
}

const StageModel& Stage::getModel() const {
    return model;
}

const std::vector<StageBackgroundElement>& Stage::getBackgroundElements() const {
    return background_elements;
}

const std::vector<Platform>& Stage::getPlatforms() const {
    return platforms;
}