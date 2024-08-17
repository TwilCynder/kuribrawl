#pragma once
#include "Debug.h"
#include "inputs.h"
#include "Util/util.h"
#include <iostream>

namespace Debug
{

} // namespace Debug

std::ostream& operator<<(std::ostream&, Input input);
std::ostream& operator<<(std::ostream&, Kuribrawl::Direction);
std::ostream& operator<<(std::ostream&, Kuribrawl::DirectionIG);
