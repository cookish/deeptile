//
// Created by Martin Cook on 2018/11/29.
//

#ifndef MLTWO_SCORERINTERFACE_HH
#define MLTWO_SCORERINTERFACE_HH

#include "BoardHandler.hh"

class ScorerInterface {
public:
    virtual double getScore(Board board) = 0;
    virtual ~ScorerInterface() = default;
};

#endif //MLTWO_SCORERINTERFACE_HH
