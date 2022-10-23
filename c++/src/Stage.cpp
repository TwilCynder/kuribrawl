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
    platforms.emplace_back(w, x, y);
}

Stage::Values::Values() : 
    size{0, 0}
{
}