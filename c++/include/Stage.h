#include <vector>
#include "AnimationsPool.h"
#include "Animation.h"
#include "Platform.h"
#include "Util/util.h"

//class Platform;

class Stage : public AnimationsPool<Animation> {

    public:
    struct Values {
        Kuribrawl::Size size;
        Kuribrawl::Rectangle camera_bounds;

        Values();
    };

    void setDisplayName(const char* name_);
    const std::string& getDisplayName() const;

    using PlatformConstIterator = std::vector<Platform>::const_iterator;
    Platform& addPlatform(int w, int x, int y);
    PlatformConstIterator getPlatforms() const;

    Values values;

    private:
    Stage(const std::string name_);

    const std::string name;   ///< Internal identifier of this Stage.
    std::string display_name;   ///< Name that will be displayed for this Stage.

    std::vector<Platform> platforms;
};