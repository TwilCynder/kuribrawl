//Inspired by a similar system in the Solarus game engine  
//https://www.solarus-games.org/fr/

#pragma once

#include <map>
#include <string>
#include <functional>

template <typename E>
requires std::is_enum_v<E>
class EnumTypeTraits {
    public:
    using names_type = std::map<E, std::string>;
};

/**
 * @brief Specify this struct with a "names" membre of type EnumTypeTraits<E>::names_type;
 * 
 * @tparam E 
 */
template <typename E>
requires std::is_enum_v<E>
struct EnumInfo {};

template <typename E>
requires std::is_enum_v<E>
EnumTypeTraits<E>::names_type enum_names;

template <typename E>
concept NamedEnum = requires {
    EnumInfo<E>::names;
};


template <typename E>
requires std::is_enum_v<E> && NamedEnum<E>
constexpr const char* enum_name(E evalue){
    auto it = enum_names<E>.find(evalue);

    return (it == enum_names<E>.end()) ? 
        nullptr : 
        it->second.c_str();
}

template <typename E>
requires std::is_enum_v<E>
constexpr auto log_enum_name(E evalue){
    return [evalue](std::ostream& os){
        auto it = enum_names<E>.find(evalue);

        if (it == enum_names<E>.end()){
            os << (int)evalue << " (enum name not found)";
        } else {
            os << it->second;
        }
    };
}