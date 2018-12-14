//
// Created by Martin Cook on 2018-12-04.
//

#ifndef MLTWO_GAMERESULT_HH
#define MLTWO_GAMERESULT_HH

#include "BoardHandler.hh"

#include <string>
using std::string;

#include <unordered_map>
using std::unordered_map;

class GameStats
{
public:
    void addVal(string name, int val = 1);
    void addVal(string name, double val);
    void setVal(string name, int val = 1);
    void setVal(string name, double val);
    void addValForGen(string name, size_t gen, int val = 1);
    Board startBoard = 0;
    Board finalBoard = 0;
    bool passedCriticalPoint = false;
    unordered_map<string, int> intValues;
    unordered_map<string, vector<int> > intValuesPerGen;
    unordered_map<string, double> doubleValues;

    size_t numGens = 0;

//    double score = 0.;
//    int boardTotal = 0;
//    double timeTaken = 0.;
//    int moves = 0;
//    int cachedEvals = 0;
//    int leafEvals = 0;
//    int totalEvals = 0;
//    int fastMoveProbCalcs = 0;

//    vector<int> cacheHitsPerGen;
//    vector<int> cacheMissesPerGen;
//    vector<int> totalEvalsPerGen;
//    vector<int> cachedEvalsPerGen;
//    vector<int> nodesPerGen;
//    vector<int> moveProbCalcsPerGen;
//    void printResult() const {
//        cout << "Total: " << boardTotal << ", score: " << score << ", "
//    }
};


#endif //MLTWO_GAMERESULT_HH
