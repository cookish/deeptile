//
// Created by Martin Cook on 2018/11/30.
//

#include "Utility.hh"

Utility::Utility() {
    rng = std::mt19937(rd());
}

int Utility::randInt(int max) {
    std::uniform_int_distribution<int> uni(0, max-1);
    return uni(rng);
}

bool Utility::coinToss(double prob) {
    std::bernoulli_distribution coinDistribution(prob);
    return coinDistribution(rng);
}
