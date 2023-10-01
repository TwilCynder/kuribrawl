#pragma once

class Champion;
class Port;

using PortRef = Port*; //to switch easily between * and &
using ConstPortRef = const Port*;
struct PlayerConfiguration{

    ConstPortRef port;
    Champion& champion;

    public:
    PlayerConfiguration(ConstPortRef, Champion&);
};