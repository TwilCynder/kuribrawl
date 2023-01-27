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