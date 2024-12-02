//Inspired by a similar system in the Solarus game engine  
//https://www.solarus-games.org/fr/

#pragma once

#include <map>
#include <string>
#include <functional>
#include "Util/streamUtil.h"

template <typename E>
requires std::is_enum_v<E>
using EnumNamesType = std::map<E, std::string>;
/*
class EnumTypeTraits {
    public:
    using names_type = ;
};
*/

template <typename E>
requires std::is_enum_v<E>
struct EnumInfo;


template <typename E>
requires std::is_enum_v<E>
static EnumNamesType<E> enum_names;

template <typename E>
concept NamedEnum = requires {
    //enum_names<E>;
    EnumInfo<E>::names;
};


template <typename E>
requires std::is_enum_v<E> && NamedEnum<E>
constexpr const char* enum_name(E evalue){
    auto it = EnumInfo<E>::names.find(evalue);

    return (it == EnumInfo<E>::names.end()) ? 
        nullptr : 
        it->second.c_str();
}

template <typename E>
requires std::is_enum_v<E>
constexpr auto log_enum_name(E evalue){
    return [evalue](std::ostream& os){
        auto it = EnumInfo<E>::names.find(evalue);

        if (it == EnumInfo<E>::names.end()){
            os << (int)evalue << " (enum name not found)";
        } else {
            os << it->second;
        }
    };
}

template <typename E>
requires std::is_enum_v<E> && NamedEnum<E>
std::ostream& operator<<(std::ostream& os, E evalue){
    return os << log_enum_name(evalue);
}

template <typename E>
requires std::is_enum_v<E> && (!NamedEnum<E>)
std::ostream& operator<<(std::ostream& os, E evalue){
    return os << (int)evalue;
}
