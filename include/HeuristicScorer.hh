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
    int sumZigZag(Board board) const;
    int sumAlongLongestMonotonicPath(Board board) const;
    int sumAlongLongestMonotonicCornerPath(Board board) const;
    int sumAlongLongestMonotonicPathRecurse(Board board, int currentPos, int parentVal, int indent) const;
    vector<int> getNeighbours(int pos) const;
    std::shared_ptr<BoardHandler> bh;
};


#endif //MLTWO_HEURISTICSCORER_HH
