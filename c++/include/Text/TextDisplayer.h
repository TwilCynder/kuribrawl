#pragma once

#include "util.h"
#include <string>


class TextureFont;

class TextDisplayer {
    protected:
    Kuribrawl::Vector pos;
    TextureFont& font;

    inline void displayString(const std::string&);

    public:
    TextDisplayer(int x, int y, TextureFont& font);
    
    TextDisplayer& operator<<(const std::string& s);
    TextDisplayer& operator<<(int i);
    TextDisplayer& operator<<(double d);

    using TextDisplayerFunctor = void (*)(TextDisplayer&);
    TextDisplayer& operator<<(TextDisplayerFunctor);

    using TextDisplayerManipulator = TextDisplayer& (&)(TextDisplayer);
    TextDisplayer& operator<<(TextDisplayerManipulator);  

    const Kuribrawl::Vector& getPosition() const;
    void setPosition(int x, int y);
    void advance(int x);

    using SetPositionData = Kuribrawl::Vector;
    static SetPositionData changePos(int x, int y);
    TextDisplayer& operator<<(SetPositionData&);
};

/**
 * @brief A TextDisplayer, but, you know, more good
 * 
 */
class AdvancedTextDisplayer : public TextDisplayer {
    protected:
    int last_advance;

    public:

    AdvancedTextDisplayer(int x, int y, TextureFont& font);

    template<typename T>
    AdvancedTextDisplayer& operator<<(T val){
        last_advance = pos.x;
        TextDisplayer::operator<<(val);
        last_advance = pos.x - last_advance;
        return *this;
    }

    using AdvanceLength = int;
    static AdvanceLength force_advance(int x);
    AdvancedTextDisplayer& operator<<(AdvanceLength);

};