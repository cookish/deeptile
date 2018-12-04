//
// Created by Martin Cook on 2018/11/30.
//

#ifndef MLTWO_EXPECTIMAX_HH
#define MLTWO_EXPECTIMAX_HH

#include "BoardHandler.hh"
#include "Utility.hh"
#include "ScorerInterface.hh"
#include "ScoreCache.hh"

using std::unique_ptr;
using std::shared_ptr;

class ExpectiMax
{
public:
    ExpectiMax(shared_ptr<BoardHandler> bh,
                   shared_ptr<Utility> utility,
                   unique_ptr<ScorerInterface> scorer,
                   unique_ptr<ScoreCache> cache)
        : bh(std::move(bh)),
          utility(std::move(utility)),
          scorer(std::move(scorer)),
          cache(std::move(cache))
    {;}
    double getBestMoveRecurse(Board board, int &move, int gens, int &numEvals, int indent = 0);
    double getAverageSpawnRecurse(Board board, int gens, int &numEvals, int indent = 0);
    double scoreForDeath = 0;

private:
    shared_ptr<BoardHandler> bh;
    shared_ptr<Utility> utility;
    unique_ptr<ScorerInterface> scorer;
    unique_ptr<ScoreCache> cache;

};


#endif //MLTWO_EXPECTIMAX_HH
