#include "EnumInfo.h"

namespace EnumTools {
    template<typename E>
    const std::string& enum_to_name(E value){
        const auto& it = EnumInfo<E>::names.find(value);
        return it.second;
    }
}