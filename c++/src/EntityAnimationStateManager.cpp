#include "EntityAnimationStateManager.h"
#include "Display/EntityAnimation.h"

void EntityAnimationStateManager::animationStarted(const EntityAnimation &anim)
{
    currentLandingWindow = anim.gameplay_behavior.getLandingBehavior().begin();
}

void EntityAnimationStateManager::frameChanged(const EntityAnimation &anim, frame_index_t frame_index){

    //Debug::log("======= FRAME CHANGED ======");
    //Debug::log(frame_index);

    auto& windows = anim.gameplay_behavior.getLandingBehavior();
    auto next_window = currentLandingWindow + 1;
    if (currentLandingWindow == windows.end() || next_window == windows.end()){
        //Debug::log("Null landing frame");
        return;
    }
    //Debug::log(currentLandingWindow->frame);
    frame_index_t next_frame = next_window->frame;
    if (frame_index >= next_frame){
        currentLandingWindow = next_window;
    }
    //Debug::log(currentLandingWindow->frame);
}
