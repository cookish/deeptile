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
    double getBestMoveRecurse(Board board, int &move, int gens, int &numEvals, double prob, int indent = 0);
    double getAverageSpawnRecurse(Board board, int gens, int &numEvals, double prob, int indent = 0);
    double scoreForDeath = 0;
    unique_ptr<GameStats> getFinalStats() { return std::move(stats); }
    void pruneCache(Board board);

    int createTree(Board board, int gens);
    void printTree();
    void scoreLeaves();
    double evaluateTree();
    int getBestMove(Board &newBoard);

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
    unordered_map<Board, double> getPrunedSpawns(const Board board, const double prob) const;

    struct BoardMoveProb {
        Board board;
        int move;
        double prob;
        BoardMoveProb(Board board, int move, double prob) : board(board), move(move), prob(prob) {;}
    };
    struct BoardProb {
        Board board;
        double prob;
        BoardProb(Board board, double prob ) : board(board), prob(prob) {;}
    };
    vector<ExpectiMax::BoardMoveProb>
    getPrunedMoves(const double prob, const std::vector<BoardAndMove> &possibleMoves, int gens) const;
    vector<BoardProb> getPrunedMoves2(Board board, const double prob, int gens) const;


    using BoardList = vector<Board>;
    using BoardProbList = vector<BoardProb>;
    vector<unordered_map<Board, BoardList> > spawnedBoardChildren;
    vector<unordered_map<Board, BoardProbList> > movedBoardChildren;
    unordered_map<Board, double> spawnedBoardsToProcess;
    unordered_map<Board, double> movedBoardsToProcess;
    vector<unordered_map<Board, double> > genMvdScores;
    vector<unordered_map<Board, double> > genSpwndScores;

};


#endif //MLTWO_EXPECTIMAX_HH
