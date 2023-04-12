#pragma once

#include <vector>
#include "Animation/AnimationsPool.h"
#include "Animation/Animation.h"
#include "PlatformModel.h"
#include "StageBackgroundElementModel.h"
#include "Util/util.h"

//class Platform;

class StageModel : public AnimationsPool<Animation> {

    public:
    struct Values {
        Kuribrawl::Size size;
        Kuribrawl::Rectangle camera_bounds;

        Values();
    };

    StageModel(const std::string name_);

    void setDisplayName(const char* name_);
    const std::string& getDisplayName() const;
    
    using PlatformModelConstIterator = std::vector<PlatformModel>::const_iterator;
    PlatformModel& addPlatform(int w, int x, int y);
    PlatformModel& addPlatform(int w, int x, int y, std::string&& animation_name);
    const std::vector<PlatformModel>& getPlatforms() const;

    StageBackgroundElementModel& addBackgroundElement(std::string&& animation_name);
    const std::vector<StageBackgroundElementModel>& getBackgroundElements() const;

    Values values;

    private:

    const std::string name;   ///< Internal identifier of this StageModel.
    std::string display_name;   ///< Name that will be displayed for this StageModel.

    std::vector<PlatformModel> platforms;
    std::vector<StageBackgroundElementModel> backgroung_elements;
};