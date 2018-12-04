//
// Created by Martin Cook on 2018/11/30.
//

#ifndef MLTWO_UTILITY_HH
#define MLTWO_UTILITY_HH

#include <random>

class Utility
{
public:
    Utility();
    int randInt(int max);
    bool coinToss(double prob);

private:
    std::mt19937 rng;
};


#endif //MLTWO_UTILITY_HH
