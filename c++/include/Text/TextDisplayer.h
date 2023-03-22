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

#include <string>
#include "Util/util.h"
#include "Util/type_wrapper.h"

class TextureFont;

/**
 * @brief Object writing text on screen, with a persistent cursos position.  
 * Currently only available with TextureFonts. 
 * 
 */
class TextDisplayer {
    protected:
    Kuribrawl::Vector pos;  /**<Current write position*/
    TextureFont& font;      /**TextureFont used to write.*/

    inline void displayString(const std::string&);

    public:

    TextDisplayer(int x, int y, TextureFont& font);
    
    TextDisplayer& operator<<(const std::string& s);
    TextDisplayer& operator<<(int );
    TextDisplayer& operator<<(double);
    TextDisplayer& operator<<(long double);

    /**Function that can be used with the << operator, returning nothing. Will be called with the current TextDisplayer, see operator<<(TextDisplayerFunctor).*/
    using TextDisplayerFunctor = void (*)(TextDisplayer&);  //maybe remplacer * par &
    TextDisplayer& operator<<(TextDisplayerFunctor);

    /**Function that can be used with the << operator, returning a reference to potentially another TextDisplayer. 
     * Will be called with the current TextDisplayer and can be used to change the current TextDisplayer in a << chain.*/
    using TextDisplayerManipulator = TextDisplayer& (&)(TextDisplayer);
    TextDisplayer& operator<<(TextDisplayerManipulator);  

    const Kuribrawl::Vector& getPosition() const;
    void setPosition(int x, int y);
    void advance(int x);

    /** Object passed to a TextDisplayer through the << operator to change the current write position.
     */
    using SetPositionData = Kuribrawl::Vector;
    static SetPositionData position(int x, int y);
    TextDisplayer& operator<<(SetPositionData&&);
};

/**
 * @brief TextDisplayer that remembers its starting point.
 */
class AnchoredTextDisplayer : public TextDisplayer {
    Kuribrawl::Vector orig_pos; /**Origin point of this textdisplayer, the position can be reset to this point.*/
    public:

    
    AnchoredTextDisplayer(int x, int y, TextureFont& font);

    void reset();
    void setOriginPosition(int x, int y);
    void setOriginPosition(const Kuribrawl::Vector&);
    const Kuribrawl::Vector& getOriginPosition() const;
};

/**
 * @brief A (anchored) TextDisplayer, but, you know, more good.
 * 
 */
class AdvancedTextDisplayer : public AnchoredTextDisplayer {
    protected:
    int last_advance; /**Length of the last string displayed.*/  

    public:

    AdvancedTextDisplayer(int x, int y, TextureFont& font);

    /**
     * @brief Redefinition of all operator<< overloads, doing some processing in addition to calling the original overload defined by TextDisplayer.
     * Updates the last_advance attribute.
     * @tparam T
     * @param val 
     * @return AdvancedTextDisplayer& 
     */
    template<typename T>
    AdvancedTextDisplayer& operator<<(T val){
        last_advance = pos.x;
        TextDisplayer::operator<<(val);
        last_advance = pos.x - last_advance;
        return *this;
    }

    struct advanceLength_d {};
    using AdvanceLength = type_wrapper<int, advanceLength_d>;
    static AdvanceLength force_advance(int x);
    AdvancedTextDisplayer& operator<<(AdvanceLength);

};