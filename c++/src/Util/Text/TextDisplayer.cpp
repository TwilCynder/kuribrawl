#include "Util/Text/TextureFont.h"
#include "Util/Text/TextDisplayer.h"

/**
 * @brief Construct a new Text Displayer.
 * @param x initial cursos position, where text will be written (top left of first char)
 * @param y initial cursos position, where text will be written (top left of first char)
 * @param font bitmap font used
 */
TextDisplayer::TextDisplayer(int x, int y, TextureFont& font_) :
    pos{x, y},
    font(font_)  
{
}

/**
 * @brief Displays a string (at the current position)
 * 
 * @param s 
 */
inline void TextDisplayer::displayString(const std::string& s){
    font.displayStringRef(s, pos.x, pos.y);
}

/**
 * @brief Overload allowing to use << to display a string.
 * 
 * @param s 
 * @return TextDisplayer& : returns a reference to this so we can keep using more <<
 */
TextDisplayer& TextDisplayer::operator<<(const std::string& s){
    displayString(s);
    return *this;
}

/**
 * @brief Overload allowing to use << to display a double.
 * 
 * Converts using std::to_string
 * 
 * @param s 
 * @return TextDisplayer& : returns a reference to this so we can keep using more <<
 */
TextDisplayer& TextDisplayer::operator<<(double d){
    displayString(std::to_string(d));
    return *this;
}

/**
 * @brief Overload allowing to use << to display a long double.
 * 
 * Converts using std::to_string
 * 
 * @param s 
 * @return TextDisplayer& : returns a reference to this so we can keep using more <<
 */
TextDisplayer& TextDisplayer::operator<<(long double d){
    displayString(std::to_string(d));
    return *this;
}

/**
 * @brief Overload allowing to use << to display an intn.
 * 
 * Converts using std::to_string
 * 
 * @param s 
 * @return TextDisplayer& : returns a reference to this so we can keep using more <<
 */
TextDisplayer& TextDisplayer::operator<<(int i){
    displayString(std::to_string(i));
    return *this;
}

/**
 * @brief Calls a specified function with this as the argument.
 * 
 * @param f 
 * @return TextDisplayer&  : returns a reference to this so we can keep using more <<
 */
TextDisplayer& TextDisplayer::operator<<(TextDisplayerFunctor f){
    f(*this);
    return *this;
}

/**
 * @brief Calls a specified function with this as the argument, returnning a TextDisplayer reference returned by that function.
 * It may be a reference to another TextDisplayer, in which case subsequent uses of << will use this other TextDisplayer.
 * 
 * @param m 
 * @return TextDisplayer& : value returned by m.
 */
TextDisplayer& TextDisplayer::operator<<(TextDisplayerManipulator m){
    return m(*this);
}

/**
 * @brief Returns the current write position
 * 
 * @return const Kuribrawl::Vector& 
 */
const Kuribrawl::Vector& TextDisplayer::getPosition() const {
    return pos;
}

/**
 * @brief Changes the current write position.
 * 
 * Theorically, you don't need to use this.
 * 
 * @param x_ 
 * @param y_ 
 */
void TextDisplayer::setPosition(int x_, int y_){
    pos.x = x_;
    pos.y = y_;
}

/**
 * @brief Increases the x write position.
 * 
 * @param x_ 
 */
void TextDisplayer::advance(int x_){
    pos.x += x_;
}

/**
 * @brief Returns a tuple comprising the specified coordinates, to be passed to operator<<(TextDisplayer::SetPositionData&&)
 * 
 * @param x 
 * @param y 
 * @return TextDisplayer::SetPositionData 
 */
TextDisplayer::SetPositionData TextDisplayer::position(int x, int y){
    return  TextDisplayer::SetPositionData{x, y};
}

/**
 * @brief Changes the current write position using a coordinates tuple (that can be constructed by TextDisplayer::position)
 * 
 * @param new_pos 
 * @return TextDisplayer&  : returns a reference to this so we can keep using more <<
 */

TextDisplayer& TextDisplayer::operator<<(TextDisplayer::SetPositionData&& new_pos){
    pos = new_pos;
    return *this;
}

/**
 * @brief Construct a new Anchored Text Displayer, see TextDisplayer(). The initial position is remembered as the origin position.
 * @param x initial cursos position and origin position, where text will be written (top left of first char)
 * @param y initial cursos position and origin position, where text will be written (top left of first char)
 * @param font bitmap font used
 */
AnchoredTextDisplayer::AnchoredTextDisplayer(int x, int y, TextureFont& font) : 
    TextDisplayer(x, y, font),
    orig_pos {x, y}
{
}

/**
 * @brief Reset the current write position to the original position.
 * 
 */
void AnchoredTextDisplayer::reset(){
    pos.x = orig_pos.x;
    pos.y = orig_pos.y;
}

/**
 * @brief Change the origin position to the specified coordinates.
 * 
 * @param x 
 * @param y 
 */
void AnchoredTextDisplayer::setOriginPosition(int x, int y){
    orig_pos.x = x;
    orig_pos.y = y;
}

/**
 * @brief Change the origin position to the specified point.
 * 
 * @param pos Vector/Tuple with xy coordinates.
 */
void AnchoredTextDisplayer::setOriginPosition(const Kuribrawl::Vector& pos){
    orig_pos = pos;
}

/**
 * @brief Exposes the origin position.
 * 
 * @return const Kuribrawl::Vector& a Vector/Tuple containing the xy coordinates.
 */
const Kuribrawl::Vector& AnchoredTextDisplayer::getOriginPosition() const {
    return orig_pos;
}

/**
 * @brief Construct a new Advanced Text Displayer.
 * 
 * @param x 
 * @param y 
 * @param font 
 */
AdvancedTextDisplayer::AdvancedTextDisplayer(int x, int y, TextureFont& font):
    AnchoredTextDisplayer(x, y, font)
{
}

/**
 * @brief Returns a value to be passed to operator<<(AdvancedTextDisplayer::AdvanceLength). 
 * Increases the current horizontal write position to be at a certain distance from the position before the previous write.
 * @param l 
 * @return AdvancedTextDisplayer::AdvanceLength 
 */
AdvancedTextDisplayer::AdvanceLength AdvancedTextDisplayer::force_advance(int l){
    return (AdvanceLength)l;
}

/**
 * @brief Increases the current horizontal write position to be at a certain distance from the position before the previous write.
 * 
 * @param l 
 * @return AdvancedTextDisplayer& 
 */
AdvancedTextDisplayer& AdvancedTextDisplayer::operator<<(AdvancedTextDisplayer::AdvanceLength l){
    if (last_advance < l.v){
        advance(l.v - last_advance);
    }
    return *this;
}