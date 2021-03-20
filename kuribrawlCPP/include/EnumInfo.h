#pragma once
#include <map>
#include <string>

namespace EnumTools {
    template <typename E>
    class EnumInfo {
        public:

        static std::map<E, std::string> names;
    };

    template <typename E>
    const std::string& enum_to_name(E value);

    #include "EnumInfo.inl"
}

