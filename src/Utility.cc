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
