/**
 * @file TextDisplayer.h
 * @author Téo Tinarrage (twilcynder@gmail.com)
 * @brief TODO
 * @version 0.1
 * @date 2021-12-05
 * @todo Features in advanced TD : 
 * - "\n" support
 * - std::hex equivalent
 * - numbers of decimals
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "Util/util.h"
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
    TextDisplayer& operator<<(int );
    TextDisplayer& operator<<(double);
    TextDisplayer& operator<<(long double);

    using TextDisplayerFunctor = void (*)(TextDisplayer&);
    TextDisplayer& operator<<(TextDisplayerFunctor);

    using TextDisplayerManipulator = TextDisplayer& (&)(TextDisplayer);
    TextDisplayer& operator<<(TextDisplayerManipulator);  

    const Kuribrawl::Vector& getPosition() const;
    void setPosition(int x, int y);
    void advance(int x);

    using SetPositionData = Kuribrawl::Vector;
    static SetPositionData position(int x, int y);
    TextDisplayer& operator<<(SetPositionData&&);
};

/**
 * @brief AdvancedTextDisplayer that remembers its starting point.
 */
class AnchoredTextDisplayer : public TextDisplayer {
    const Kuribrawl::Vector orig_pos;
    public:

    AnchoredTextDisplayer(int x, int y, TextureFont& font);

    void reset();
};

/**
 * @brief A TextDisplayer, but, you know, more good
 * 
 */
class AdvancedTextDisplayer : public AnchoredTextDisplayer {
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