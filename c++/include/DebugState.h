#pragma once
#include "Fighter.h"
#include <iostream>

class Fighter;

namespace Debug
{
    void log(Fighter::State state);
    void log(Fighter::State state, int frame);
    void log(Fighter::State state, Fighter& fighter);
    void log(Fighter& fighter);
} // namespace Debug
