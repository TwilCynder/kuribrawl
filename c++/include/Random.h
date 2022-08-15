#pragma once

class Random {
    static long seed;
    static void nextSeed();

    public:
    static void setSeed(long);
    static int get_int();
};