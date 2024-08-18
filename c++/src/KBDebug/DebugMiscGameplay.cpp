#include "KBDebug/DebugMiscGameplay.h"
#include <map>
#include <string>
#include "Util/EnumInfo.h"
#include "Util/streamUtil.h"

template<>
struct EnumInfo<Input>{static const EnumTypeTraits<Input>::names_type names;};
const EnumTypeTraits<Input>::names_type EnumInfo<Input>::names = {
    {Input::NONE, "None"},
    {Input::ATTACK, "Attack"},
    {Input::SPECIAL, "Special"},
    {Input::JUMP, "Jump"},
    {Input::SHORTHOP, "Shorthop"},
    {Input::SMASH, "Smash"},
    {Input::UP, "Up"},
    {Input::LEFT, "Left"},
    {Input::DOWN, "Down"},
    {Input::RIGHT, "Right"}
};

template<>
struct EnumInfo<Kuribrawl::Direction>{static const EnumTypeTraits<Kuribrawl::Direction>::names_type names;};
const EnumTypeTraits<Kuribrawl::Direction>::names_type EnumInfo<Kuribrawl::Direction>::names = {
    {Kuribrawl::Direction::NONE, "Neutral"},
    {Kuribrawl::Direction::LEFT, "Left"},
    {Kuribrawl::Direction::RIGHT, "Right"},
    {Kuribrawl::Direction::UP, "Up"},
    {Kuribrawl::Direction::DOWN, "Down"}
};

template<>
struct EnumInfo<Kuribrawl::DirectionIG>{static const EnumTypeTraits<Kuribrawl::DirectionIG>::names_type names;};
const EnumTypeTraits<Kuribrawl::DirectionIG>::names_type EnumInfo<Kuribrawl::DirectionIG>::names = {
    {Kuribrawl::DirectionIG::NONE, "Neutral"},
    {Kuribrawl::DirectionIG::FORWARD, "Forward"},
    {Kuribrawl::DirectionIG::BACK, "Back"},
    {Kuribrawl::DirectionIG::UP, "Up"},
    {Kuribrawl::DirectionIG::DOWN, "Down"}
};

/**
 * @brief Logs a game Input to a stream.  
 * Tries to find its name in the EnumInfo<E>::names table.
 * 
 * @param input 
 */
std::ostream &operator<<(std::ostream &os , Input input)
{
    return os << log_enum_name(input);
}

std::ostream &operator<<(std::ostream &os, Kuribrawl::Direction dir)
{
    return os << log_enum_name(dir);
}

std::ostream &operator<<(std::ostream &os, Kuribrawl::DirectionIG dir)
{
    return os << log_enum_name(dir);
}
