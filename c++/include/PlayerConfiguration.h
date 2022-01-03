#pragma once

class Champion;
class Port;

using PortRef = Port*; //to switch easily between * and &
struct PlayerConfiguration{

    PortRef port;
    Champion& champion;

    public:
    PlayerConfiguration(PortRef, Champion&);
};