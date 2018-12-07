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
    double getBestMoveRecurse(Board board, int &move, int gens, int &numEvals, double prob, int indent = 0);
    double getAverageSpawnRecurse(Board board, int gens, int &numEvals, double prob, int indent = 0);
    double scoreForDeath = 0;
    unique_ptr<GameStats> getFinalStats() { return std::move(stats); }
    void pruneCache(Board board);

private:
    shared_ptr<BoardHandler> bh;
    shared_ptr<Utility> utility;
    unique_ptr<ScorerInterface> scorer;
    unique_ptr<ScoreCache> cache;
    unique_ptr<GameStats> stats;
    struct TilePosProb {
        int tile;
        int pos;
        double prob;
        TilePosProb(int tile, int pos, double prob) : tile(tile), pos(pos), prob(prob) {;}
    };
    vector<TilePosProb> getPrunedSpawns(Board board, double prob) const;

    struct BoardMoveProb {
        Board board;
        int move;
        double prob;
        BoardMoveProb(Board board, int move, double prob) : board(board), move(move), prob(prob) {;}
    };
    vector<ExpectiMax::BoardMoveProb>
    getPrunedMoves(const double prob, const std::vector<BoardAndMove> &possibleMoves, int gens) const;
};


#endif //MLTWO_EXPECTIMAX_HH
