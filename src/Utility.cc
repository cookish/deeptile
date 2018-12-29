//
// Created by Martin Cook on 2018/11/30.
//

#include "Utility.hh"

// see https://codereview.stackexchange.com/questions/109260/seed-stdmt19937-from-stdrandom-device
Utility::Utility()
  : rng(std::random_device{}())
{
    rng.discard(700000);
}

int Utility::randInt(int max) {
    std::uniform_int_distribution<int> uni(0, max-1);
    return uni(rng);
}

bool Utility::coinToss(double prob) {
    std::bernoulli_distribution coinDistribution(prob);
    return coinDistribution(rng);
}


string Utility::getMoveName(int move) {
    switch(move) {
        case 0: return "left";
        case 1: return "up";
        case 2: return "right";
        case 3: return "down";
        default:
            return "error";
    }
}
