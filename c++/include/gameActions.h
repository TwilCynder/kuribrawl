#pragma once

/**
 * @brief Distinct actions a player can take.
 * 
 */
enum commands {
    Jab,
    FTilt,
    UTilt,
    DTilt,
    FSmash,
    USmash,
    DSmash,
    DashAttack,
    NAir,
    Fair,
    BAir,
    UAir,
    DAir,
    ZAir,
    NSpecial,
    SSpecial,
    USpecial,
    DSpecial,
    Grab,
    Guard,
    Jump,
    Dash,
    Walk,
    Roll,
    Spotdodge,
    AirDodge,
    DirectionalAirDodge
};

/**
 * @brief Types of jump height.
 * 
 */
enum jumpY {
    Full,
    Short,
    Air,
};

/**
 * @brief Types of jump angle.
 * 
 */
enum jumpX {
    Forward,
    Normal,
    Backwards
};