//
// Created by Martin Cook on 2018-12-04.
//

#include "GameStats.hh"

GameStats::GameStats()
    : cacheHitsPerGen(vector<int>(20)),
      cacheMissesPerGen(vector<int>(20)),
      totalEvalsPerGen(vector<int>(20)),
      cachedEvalsPerGen(vector<int>(20)),
      nodesPerGen(vector<int>(20)),
      moveProbCalcsPerGen(vector<int>(20))
{;}
