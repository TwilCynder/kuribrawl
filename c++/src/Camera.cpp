#include "Camera.h"
#include "defs.h"

constexpr int half_screen_w = SCREEN_WIDTH / 2;
constexpr int half_screen_h = SCREEN_HEIGHT / 2;


Camera::Camera() :
    position{0, 0}
{}

int Camera::getXOnScreen(int x) const {
    return x - position.x + half_screen_w;
}

int Camera::getYOnScreen(int y) const {
    return SCREEN_HEIGHT - (y - position.y);
}

int Camera::getXOnScreen(int x, double depth) const {
    return (depth <= 0.0 ? x : (x - position.x) / depth) + half_screen_w;
}

int Camera::getYOnScreen(int y, double depth) const {
    return SCREEN_HEIGHT - (depth <= 0.0 ? y : (y - position.y) / depth);
}