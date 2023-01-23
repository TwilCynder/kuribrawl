#pragma once

#include <vector>
#include "StageBackgroundElement.h"

class StageModel;
class StageBackgroundElementModel;
class Game;

class Stage {
    public:
    
    private:
    const StageModel& model;
    std::vector<StageBackgroundElement> background_elements;

    public:
    Stage(const StageModel&);
    const StageModel& getModel() const;

    const std::vector<StageBackgroundElement> getBackgroundElements() const;
};