#include "Text/TextureFont.h"
#include "Text/TextDisplayer.h"
#include "Debug.h"

TextDisplayer::TextDisplayer(int x, int y, TextureFont& font_) :
    pos{x, y},
    font(font_)  
{
}

TextDisplayer& TextDisplayer::operator<<(std::string s){
    font.displayString(s, pos.x, pos.y);
    return *this;
}

TextDisplayer& TextDisplayer::operator<<(double d){
    font.displayString(std::to_string(d), pos.x, pos.y);
    return *this;
}

TextDisplayer& TextDisplayer::operator<<(int i){
    font.displayString(std::to_string(i), pos.x, pos.y);
    return *this;
}

TextDisplayer& TextDisplayer::operator<<(TextDisplayerFunctor f){
    f(*this);
    return *this;
}

TextDisplayer& TextDisplayer::operator<<(TextDisplayerManipulator m){
    return m(*this);
}

const Kuribrawl::Vector& TextDisplayer::getPosition() const {
    return pos;
}

void TextDisplayer::setPosition(int x_, int y_){
    pos.x = x_;
    pos.y = y_;
}

void TextDisplayer::advance(int x_){
    pos.x += x_;
}