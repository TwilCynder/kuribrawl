#include "Random.h"

#define RAND_MAX 4294967296
#define RAND_MOD 4294967295
#define multiplier 214013 
#define increment 2531011 

long Random::seed = 12;

void Random::setSeed(long s){
    seed = s;
}

void Random::nextSeed(){
    seed = seed * multiplier + increment % RAND_MAX; //askip ça c'est plus opti, ça fait une instruction de moins, wtf
    //seed = (seed * multiplier + increment) & RAND_MOD;
}

int Random::get_int(){
    nextSeed();
    return seed >> 16;
}