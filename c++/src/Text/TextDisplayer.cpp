#include "Text/TextureFont.h"
#include "Text/TextDisplayer.h"
#include "Debug.h"

TextDisplayer::TextDisplayer(int x, int y, TextureFont& font_) :
    pos{x, y},
    font(font_)  
{
}

inline void TextDisplayer::displayString(const std::string& s){
    font.displayString(s, pos.x, pos.y);
}

TextDisplayer& TextDisplayer::operator<<(const std::string& s){
    displayString(s);
    return *this;
}

TextDisplayer& TextDisplayer::operator<<(double d){
    displayString(std::to_string(d));
    return *this;
}

TextDisplayer& TextDisplayer::operator<<(int i){
    displayString(std::to_string(i));
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

TextDisplayer::SetPositionData TextDisplayer::changePos(int x, int y){
    return {x, y};
}

TextDisplayer& TextDisplayer::operator<<(TextDisplayer::SetPositionData& new_pos){
    pos = new_pos;
    return *this;
}

AdvancedTextDisplayer::AdvancedTextDisplayer(int x, int y, TextureFont& font):
    TextDisplayer(x, y, font)
{
}

AdvancedTextDisplayer::AdvanceLength AdvancedTextDisplayer::force_advance(int l){
    return l;
}

AdvancedTextDisplayer& AdvancedTextDisplayer::operator<<(AdvancedTextDisplayer::AdvanceLength l){
    if (last_advance < l){
        advance(l - last_advance);
    }
    return *this;
}