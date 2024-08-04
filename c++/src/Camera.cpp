#include "Camera.h"
#include "defs.h"
#include "Types/GameTypes.h"

constexpr int half_screen_w = SCREEN_WIDTH / 2;
constexpr int half_screen_h = SCREEN_HEIGHT / 2;

using namespace::Kuribrawl;

Camera::Camera() :
    position{0, 0}
{}

int Camera::getXOnScreen(int x) const {
    return x - position.x + half_screen_w;
}

int Camera::getYOnScreen(int y) const {
    return SCREEN_HEIGHT - (y - position.y);
}

int Camera::getXOnScreen(int x, Types::depth_t depth) const {
    return (depth == Types::infinite_depth ? x : (x - position.x) / depth) + half_screen_w;
}

int Camera::getYOnScreen(int y, Types::depth_t depth) const {
    return SCREEN_HEIGHT - (depth == Types::infinite_depth ? y : (y - position.y) / depth);
}