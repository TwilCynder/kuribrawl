#pragma once

class Entity {
    long euid;

    protected:
    Entity();

    long makeEUID();

    public:
    long getEUID();
};