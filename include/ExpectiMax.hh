//
// Created by Martin Cook on 2018/11/30.
//

#ifndef MLTWO_EXPECTIMAX_HH
#define MLTWO_EXPECTIMAX_HH

#include "BoardHandler.hh"
#include "Utility.hh"
#include "ScorerInterface.hh"

using std::unique_ptr;
using std::shared_ptr;

class ExpectiMax
{
public:
    ExpectiMax(shared_ptr<BoardHandler> bh, shared_ptr<Utility> utility, unique_ptr<ScorerInterface> scorer)
        : bh(std::move(bh)),
          utility(std::move(utility)),
          scorer(std::move(scorer))
    {;}
    double getBestMoveRecurse(Board board, int &move, int gen, int indent = 0);
    double getAverageSpawnRecurse(Board board, int gen, int indent = 0);
    double genLimit = 4;
    double scoreForDeath = 0;

private:
    shared_ptr<BoardHandler> bh;
    shared_ptr<Utility> utility;
    unique_ptr<ScorerInterface> scorer;

};


#endif //MLTWO_EXPECTIMAX_HH
