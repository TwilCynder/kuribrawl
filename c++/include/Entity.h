#pragma once

#include <cstdint>

class Entity {
    uint32_t euid;

    protected:
    Entity();

    static uint32_t makeEUID();

    public:
    uint32_t getEUID() const;
};