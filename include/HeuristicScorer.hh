//
// Created by Martin Cook on 2018/11/29.
//

#ifndef MLTWO_HEURISTICSCORER_HH
#define MLTWO_HEURISTICSCORER_HH

#include "ScorerInterface.hh"

#include <vector>
using std::vector;

class Settings;
class HeuristicScorer : public ScorerInterface
{
public:
    HeuristicScorer(std::shared_ptr<BoardHandler> bh, const Settings* settings);
    void scoreMapOfBoards(std::unordered_map<Board, double> &map) override;
    double getScoreSpawned(Board board) override;
    double getScoreMoved(Board board) override { return getScoreSpawned(board); }
    // the board itself is a reasonable score...
    double getScoreMovedFast(Board board) override;

    double sumZigZag(Board board) const;
    double sumZigZagPlusTail(Board board) const;
    double sumAlongLongestMonotonicCornerPath(Board board) const;
    double repeatMonotonic(Board board) const;
    void setBase(double b);

private:
    void initTileScores();
    int findMaxValue(Board board) const;
    double sumAlongLongestMonotonicPath(Board board) const;
    double sumAlongLongestMonotonicPathRecurse(Board board, int currentPos, int parentVal, int indent) const;
    std::shared_ptr<BoardHandler> bh;
    const Settings* settings;

    std::array<int, 16> neighbourList;
    std::array<int, 16> belowList;
    double base = 0.;
    std::array<double, 16> tileScore;
};


#endif //MLTWO_HEURISTICSCORER_HH
