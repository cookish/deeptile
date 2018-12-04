//
// Created by Martin Cook on 2018-12-04.
//

#ifndef MLTWO_RUNSTATS_HH
#define MLTWO_RUNSTATS_HH

#include "BoardHandler.hh"
#include "GameStats.hh"

class RunStats
{
public:
    explicit RunStats(const std::vector<GameStats> &games);
    double score = 0.;
    double boardTotal = 0;
    double timeTaken = 0.;
    double moves = 0;
    double cachedEvals = 0;
    double leafEvals = 0;
    double totalEvals = 0;
    vector<double> cacheHitsPerGen;
    vector<double> cacheMissesPerGen;
    vector<double> totalEvalsPerGen;
    vector<double> cachedEvalsPerGen;
};



#endif //MLTWO_RUNSTATS_HH
