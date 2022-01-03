#include "PlayerConfiguration.h"
#include "Champion.h"
#include "Port.h"

PlayerConfiguration::PlayerConfiguration(PortRef port_, Champion& champion_):
    port(port_), 
    champion(champion_)
{
}