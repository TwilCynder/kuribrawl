#pragma once

#include "util.h"
#include <string>


class TextureFont;

class TextDisplayer {
    protected:
    Kuribrawl::Vector pos;
    TextureFont& font;

    public:
    TextDisplayer(int x, int y, TextureFont& font);
    
    TextDisplayer& operator<<(std::string s);
    TextDisplayer& operator<<(int i);
    TextDisplayer& operator<<(double d);

    using TextDisplayerFunctor = void (*)(TextDisplayer&);
    TextDisplayer& operator<<(TextDisplayerFunctor);

    using TextDisplayerManipulator = TextDisplayer& (&)(TextDisplayer);
    TextDisplayer& operator<<(TextDisplayerManipulator);  

    const Kuribrawl::Vector& getPosition() const;
    void setPosition(int x, int y);
    void advance(int x);
};