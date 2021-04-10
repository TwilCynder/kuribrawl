#pragma once

enum Input {
    NONE,
    ATTACK,
    SPECIAL,
    JUMP,
    SHORTHOP,
    GUARD,
    GRAB,
    SMASH,
    RIGHT,
    UP,
    LEFT,
    DOWN,
    TOTAL
};

enum CommonElement {
    A,
    B,
    X,
    Y,
    Z,
    L,
    R,
    START
};

enum ElementType {
    UNKNOWN = -1,
    BUTTON,
    STICK,
    TRIGGER
};