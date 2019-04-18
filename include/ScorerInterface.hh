//
// Created by Martin Cook on 2018/11/29.
//

#ifndef MLTWO_SCORERINTERFACE_HH
#define MLTWO_SCORERINTERFACE_HH

#include "BoardHandler.hh"

#include <unordered_map>

class ScorerInterface {
public:
    virtual double getScoreSpawned(Board board) = 0;
    virtual double getScoreMoved(Board board) = 0;
    virtual double getScoreMovedFast(Board board) = 0;
    virtual ~ScorerInterface() = default;
    virtual void scoreMapOfBoards(std::unordered_map<Board, double> &map) = 0;
};

#endif //MLTWO_SCORERINTERFACE_HH
