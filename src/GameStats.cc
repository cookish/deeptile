//
// Created by Martin Cook on 2018-12-04.
//

#include "GameStats.hh"

GameStats::GameStats()
    : cacheHitsPerGen(vector<int>(10)),
      cacheMissesPerGen(vector<int>(10)),
      totalEvalsPerGen(vector<int>(10)),
      cachedEvalsPerGen(vector<int>(10)),
      nodesPerGen(vector<int>(10))
{;}
