#pragma once
#include "Debug.h"
#include "inputs.h"
#include "Util/util.h"
#include <iostream>
#include "Util/EnumInfo.h"

namespace Debug
{

} // namespace Debug

template<>
struct EnumInfo<Input>{static const EnumNamesType<Input> names;};
template<>
struct EnumInfo<Kuribrawl::Direction>{static const EnumNamesType<Kuribrawl::Direction> names;};
template<>
struct EnumInfo<Kuribrawl::DirectionIG>{static const EnumNamesType<Kuribrawl::DirectionIG> names;};