#include "gameCalculations.h"

#define KNOCKBACK_BASE 10
#define KNOCKBACK_MULT 0.1

double GameCalc::getKnockback(int percentage, double bkb, double skb, double weight){
    //return bkb + (((percentage * weight) +KNOCKBACK_BASE) * skb * KNOCKBACK_MULT);
    return 6.0;
}

int GameCalc::getHitLag(double damage){
    return 15;
}