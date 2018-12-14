//
// Created by Martin Cook on 2018-12-04.
//

#ifndef MLTWO_RUNSTATS_HH
#define MLTWO_RUNSTATS_HH

#include "BoardHandler.hh"
#include "GameStats.hh"

#include <map>
#include <string>
using std::string;

#include <unordered_map>
using std::unordered_map;

class RunStats
{
public:
    explicit RunStats(const std::vector<GameStats> &games);
    std::map<string, double> values;
    unordered_map<string, vector<double> > valuesPerGen;
    void printRateInfo();

private:
    size_t numGens = 0;
};



#endif //MLTWO_RUNSTATS_HH
