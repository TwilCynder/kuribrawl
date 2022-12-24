#include "Stage.h"

Stage::Stage(const std::string name_): 
    name(name_)
{
}

void Stage::setDisplayName(const char* name_){
    display_name.assign(name_);
}

const std::string& Stage::getDisplayName() const {
    return display_name;
}

Platform& Stage::addPlatform(int w, int x, int y){
    return platforms.emplace_back(w, x, y);
}

Stage::PlatformConstIterator Stage::getPlatforms() const {
    return platforms.begin();
}

Stage::Values::Values() : 
    size{0, 0}, camera_bounds{0, 0, 0, 0}
{
}