#include "Display/Graphics.h"

Graphics::Graphics(){
    drawer.add(main_plane);
}

void Graphics::add(const Drawable& d, const double depth, const int subdepth){
    drawer.add(d, depth, subdepth);
}

void Graphics::add(DynamicPlaneDrawable& d, const DynamicPlaneDrawable::Layer layer, const int level){
    addToMainPlane(d, layer, level);
}

void Graphics::addToMainPlane(DynamicPlaneDrawable &d, const DynamicPlaneDrawable::Layer layer, const int level)
{
    main_plane.add(d, layer, level);
}

void Graphics::draw(SDL_Renderer* t, const Camera& c)  {
    main_plane.sort();
    drawer.draw(t, c);
}