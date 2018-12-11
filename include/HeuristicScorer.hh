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
    explicit HeuristicScorer(std::shared_ptr<BoardHandler> bh);
    double getScoreSpawned(Board board) override;
    double getScoreMoved(Board board) override { return getScoreSpawned(board); }
    // the board itself is a reasonable score...
    double getScoreMovedFast(Board board) override;

    double sumZigZag(Board board) const;
    double sumAlongLongestMonotonicCornerPath(Board board) const;
    double repeatMonotonic(Board board) const;
    void setBase(double b);

private:
    void initTileScores();
    int findMaxValue(Board board) const;
    double sumAlongLongestMonotonicPath(Board board) const;
    double sumAlongLongestMonotonicPathRecurse(Board board, int currentPos, int parentVal, int indent) const;
    std::shared_ptr<BoardHandler> bh;

    std::array<int, 16> neighbourList;
    std::array<int, 16> belowList;
    double base = 2.3;
    std::array<double, 16> tileScore;
};


#endif //MLTWO_HEURISTICSCORER_HH
