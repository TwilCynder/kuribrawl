#include "Drawer.h"

#include "KBDebug/Debug.h"

void Drawer::add(const Drawable& d, const int depth){
    drawables.emplace(Element(d, depth));
}

void Drawer::add(const Drawable& d){
    add(d, 0);
}

void Drawer::draw(SDL_Renderer* target, const Camera& camera){
    for (const Element& e : drawables){
        e.drawable.draw(target, camera);
    }
}