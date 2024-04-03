#include "Display/Drawer.h"

#include "KBDebug/Debug.h"

void Drawer::add(const Drawable& d, const int depth, const Layer layer, const int level){
    drawables.emplace(Element(d, depth, layer, level));
}

void Drawer::add(const Drawable& d, const int depth, const Layer layer){
    add(d, depth, layer, 0);
}

void Drawer::add(const Drawable& d, const int depth){
    add(d, 0, MIDDLE);
}

void Drawer::add(const Drawable& d){
    add(d, 0);
}

void Drawer::draw(SDL_Renderer* target, const Camera& camera){
    for (const Element& e : drawables){
        e.drawable.draw(target, camera);
    }
}