//
// Created by Martin Cook on 2018/11/29.
//

#ifndef MLTWO_HEURISTICSCORER_HH
#define MLTWO_HEURISTICSCORER_HH

#include "ScorerInterface.hh"

#include <vector>
using std::vector;

class HeuristicScorer : public ScorerInterface
{
public:
    explicit HeuristicScorer(std::shared_ptr<BoardHandler> bh) : bh(std::move(bh)) {;}
    double getScore(Board board) override;

private:
    int findMaxValue(Board board) const;
    double sumZigZag(Board board) const;
    double sumAlongLongestMonotonicPath(Board board) const;
    double sumAlongLongestMonotonicCornerPath(Board board) const;
    double sumAlongLongestMonotonicPathRecurse(Board board, int currentPos, int parentVal, int indent) const;
    vector<int> getNeighbours(int pos) const;
    std::shared_ptr<BoardHandler> bh;
};


#endif //MLTWO_HEURISTICSCORER_HH
