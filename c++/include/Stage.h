#pragma once

#include <vector>
#include "StageBackgroundElement.h"
#include "Platform.h"

class StageModel;
class StageBackgroundElementModel;
class Game;
class GameGraphics;

class Stage {
    public:
    
    private:
    const StageModel& model;
    std::vector<StageBackgroundElement> background_elements;
    std::vector<Platform> platforms;

    public:
    Stage(const StageModel&);
    const StageModel& getModel() const;

    const std::vector<StageBackgroundElement>& getBackgroundElements() const;
    const std::vector<Platform>& getPlatforms() const;

    void addToGraphics(GameGraphics&);
};