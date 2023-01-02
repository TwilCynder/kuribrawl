#include "StageModel.h"

StageModel::StageModel(const std::string name_): 
    name(name_)
{
}

void StageModel::setDisplayName(const char* name_){
    display_name.assign(name_);
}

const std::string& StageModel::getDisplayName() const {
    return display_name;
}

PlatformModel& StageModel::addPlatform(int w, int x, int y){
    return platforms.emplace_back(w, x, y);
}

StageModel::PlatformModelConstIterator StageModel::getPlatforms() const {
    return platforms.begin();
}

StageModel::Values::Values() : 
    size{0, 0}, camera_bounds{0, 0, 0, 0}
{
}