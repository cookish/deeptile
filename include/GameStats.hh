//
// Created by Martin Cook on 2018-12-04.
//

#ifndef MLTWO_GAMERESULT_HH
#define MLTWO_GAMERESULT_HH

#include "BoardHandler.hh"

class GameStats
{
public:
    Board startBoard = 0;
    Board finalBoard = 0;
    double score = 0.;
    int boardTotal = 0;
    double timeTaken = 0.;
    int moves = 0;
    int cachedEvals = 0;
    int leafEvals = 0;
    int totalEvals = 0;
    bool passedCriticalPoint = false;
    vector<int> cacheHitsPerGen;
    vector<int> cacheMissesPerGen;
    vector<int> totalEvalsPerGen;
    vector<int> cachedEvalsPerGen;
    vector<int> nodesPerGen;
//    void printResult() const {
//        cout << "Total: " << boardTotal << ", score: " << score << ", "
//    }
    GameStats();
};


#endif //MLTWO_GAMERESULT_HH
