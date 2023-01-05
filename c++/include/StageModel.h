#pragma once

#include <vector>
#include "AnimationsPool.h"
#include "Animation.h"
#include "Platform.h"
#include "Util/util.h"

//class Platform;

class StageModel : public AnimationsPool<Animation> {

    public:
    struct Values {
        Kuribrawl::Size size;
        Kuribrawl::Rectangle camera_bounds;

        Values();
    };

    struct BackgroundElement {
        const Animation* anim;
        Kuribrawl::Vector position;
        int depth;
    };

    StageModel(const std::string name_);

    void setDisplayName(const char* name_);
    const std::string& getDisplayName() const;
    
    using PlatformModelConstIterator = std::vector<PlatformModel>::const_iterator;
    PlatformModel& addPlatform(int w, int x, int y);
    PlatformModel& addPlatform(int w, int x, int y, std::string&& animation_name);
    const std::vector<PlatformModel>& getPlatforms() const;

    BackgroundElement& addBackgroundElement(std::string&& animation_name);

    Values values;

    private:

    const std::string name;   ///< Internal identifier of this StageModel.
    std::string display_name;   ///< Name that will be displayed for this StageModel.

    std::vector<PlatformModel> platforms;
    std::vector<BackgroundElement> backgroung_elements;
};