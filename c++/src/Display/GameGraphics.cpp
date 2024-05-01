#include <algorithm>

#include "Display/GameGraphics.h"
#include "DepthfulEntity.h"

#include "KBDebug/Debug.h"

void GameGraphics::clear(){
    entities.clear();
}

void GameGraphics::add(DepthfulEntity& entity){
    entities.emplace_back(entity);
}

void GameGraphics::draw(SDL_Renderer* target, const Camera& cam){
    std::sort(entities.begin(), entities.end(), DepthfulEntityComparator());

    for (auto entity : entities){
        entity.get().draw(target, cam);
    }
}