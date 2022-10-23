#include <vector>
#include "AnimationsPool.h"
#include "Animation.h"
#include "Platform.h"
#include "Util/util.h"

//class Platform;

class Stage : public AnimationsPool<Animation> {
    public:
    struct Values {
        Kuribrawl::Rect size;
        Kuribrawl::Vector camera_limits_offset;
        Kuribrawl::Rect camera_limits_size;

        Values();
    };

    void setDisplayName(const char* name_);
    const std::string& getDisplayName() const;

    Platform& addPlatform(int w, int x, int y);

    private:
    Stage(const std::string name_);

    const std::string name;   ///< Internal identifier of this Stage.
    std::string display_name;   ///< Name that will be displayed for this Stage.
    Values values;

    std::vector<Platform> platforms;
};