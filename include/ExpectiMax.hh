//
// Created by Martin Cook on 2018/11/30.
//

#ifndef MLTWO_EXPECTIMAX_HH
#define MLTWO_EXPECTIMAX_HH

#include "BoardHandler.hh"
#include "Utility.hh"
#include "ScorerInterface.hh"
#include "ScoreCache.hh"
#include "GameStats.hh"

#include <array>
using std::array;

using std::unique_ptr;
using std::shared_ptr;

class ExpectiMax
{
public:
    ExpectiMax(shared_ptr<BoardHandler> bh,
                   shared_ptr<Utility> utility,
                   unique_ptr<ScorerInterface> scorer,
                   unique_ptr<ScoreCache> cache,
                   unique_ptr<GameStats> stats)
        : bh(std::move(bh)),
          utility(std::move(utility)),
          scorer(std::move(scorer)),
          cache(std::move(cache)),
          stats(std::move(stats))
    {;}
    double scoreForDeath = 0;
    unique_ptr<GameStats> getFinalStats() { return std::move(stats); }

    int createTree(Board board, int gens);
    void printTree();
    void scoreLeaves();
    double evaluateTree();
    void evaluteEffort();
    int getBestMove(Board board);

private:
    struct BoardProb {
        Board board;
        double prob;
        BoardProb(Board board, double prob ) : board(board), prob(prob) {;}
    };
    using BoardList = vector<Board>;
    using BoardProbList = vector<BoardProb>;

    shared_ptr<BoardHandler> bh;
    shared_ptr<Utility> utility;
    unique_ptr<ScorerInterface> scorer;
    unique_ptr<ScoreCache> cache;
    unique_ptr<GameStats> stats;

    vector<BoardProb> getPrunedMoves(Board board, double prob, int gens) const;
    unordered_map<Board, double> getPrunedSpawns(Board board, const double prob) const;

    vector<unordered_map<Board, BoardList> > spawnedBoardChildren;
    vector<unordered_map<Board, BoardProbList> > movedBoardChildren;
    unordered_map<Board, double> spawnedBoardsToProcess;
    unordered_map<Board, double> movedBoardsToProcess;
    vector<unordered_map<Board, double> > genMvdScores;
    vector<unordered_map<Board, double> > genSpwndScores;
    vector<unordered_map<Board, int> > genMvdEffort;
    vector<unordered_map<Board, int> > genSpwndEffort;

};


#endif //MLTWO_EXPECTIMAX_HH
