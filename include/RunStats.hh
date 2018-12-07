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

class RunStats
{
public:
    explicit RunStats(const std::vector<GameStats> &games);
    std::map<string, double> d;
    vector<double> cacheHitsPerGen;
    vector<double> cacheMissesPerGen;
    vector<double> totalEvalsPerGen;
    vector<double> cachedEvalsPerGen;
    vector<double> nodesPerGen;
    vector<double> moveProbCalcsPerGen;
    void printRateInfo();
    void printCacheInfo();

private:
    size_t numGens = 0;
};



#endif //MLTWO_RUNSTATS_HH
