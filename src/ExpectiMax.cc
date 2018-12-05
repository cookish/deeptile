//
// Created by Martin Cook on 2018/11/30.
//

#include "ExpectiMax.hh"

#include <iostream>
using std::cout;
using std::endl;

double ExpectiMax::getBestMoveRecurse(const Board board,
                                      int &move,
                                      const int gens,
                                      int &numEvals,
                                      const double prob,
                                      const int indent)
{
    move = -1;
    numEvals = 0;

    auto possibleMoves = bh->getPossibleMoves(board);
    if (possibleMoves.empty()) {
        return scoreForDeath;
    }

    if (gens == 0) {
        numEvals = 1;
        ++stats->leafEvals;
        return scorer->getScore(board);
    }
    //    for (int i = 0; i < indent; i++) cout << " ";
    //    cout << "getBestMoveRecurse: evaluating board:" << endl;
    //    bh->printHex(board, indent);
    auto moveOptions = getPrunedMoves(prob, possibleMoves);
    double score;
    double bestScore = scoreForDeath;
    for (const auto &moveOption : moveOptions) {
        int tmpEvals = 0;
        score = getAverageSpawnRecurse(moveOption.board, gens, tmpEvals, prob * moveOption.prob, indent + 2);
        numEvals += tmpEvals;
        if (score > bestScore) {
            move = moveOption.move;
            bestScore = score;
        }
    }
//    for (int i = 0; i < indent; i++) cout << " ";
//    cout << "getBestMoveRecurse: returning score:" << bestScore << endl;
    return bestScore;
}


double ExpectiMax::getAverageSpawnRecurse(const Board board,
                                          const int gens,
                                          int &numEvals,
                                          const double prob,
                                          int indent)
{
//    for (int i = 0; i < indent; i++) cout << " ";
//    cout << "getAverageSpawnRecurse: evaluating board:" << endl;
//    bh->printHex(board, indent);
    Board spawnedBoard;
    auto spawns = getPrunedSpawns(board, prob);

    int cachedEvals = 0;
    int cacheHits = 0;
    int cacheMisses = 0;
    int numNodes = 0;
    double score = 0;
    int tempMove = 0;
    auto newGens = gens - 1;
    for (const auto &spawn : spawns) {
        spawnedBoard = board | (static_cast<Board>(spawn.tile) << (4 * spawn.pos));
        auto pBoard = bh->getPrincipalBoard(spawnedBoard);
        auto cacheVal = cache->get(pBoard, newGens);
        ++numNodes;
        if (cacheVal.score >= 0) {  // found a cached result
            score += cacheVal.score * spawn.prob;
            cachedEvals += cacheVal.numEvals;
            ++cacheHits;
        } else {
            int tmpEvals = 0;
            auto calcScore = getBestMoveRecurse(
                spawnedBoard, tempMove, newGens, tmpEvals, prob * spawn.prob, indent + 2);
            numEvals += tmpEvals;
            score += calcScore * spawn.prob;
            cache->insertScoreSafe(pBoard, newGens, calcScore, tmpEvals);
            ++cacheMisses;
        }
    }
    numEvals += cachedEvals;
    stats->cachedEvalsPerGen[newGens] += cachedEvals;
    stats->totalEvalsPerGen[newGens] += numEvals;
    stats->cachedEvals += cachedEvals;
    stats->cacheHitsPerGen[newGens] += cacheHits;
    stats->cacheMissesPerGen[newGens] += cacheMisses;
    stats->nodesPerGen[newGens] += numNodes;

//    score = score / possibleSpawns.size();
//    for (int i = 0; i < indent; i++) cout << " ";
//    cout << "getAverageSpawnRecurse: returning score:" << score << endl;
    return score;
}

void ExpectiMax::pruneCache(Board board) {
    cache->prune(board, bh.get());
}

vector<ExpectiMax::TilePosProb> ExpectiMax::getPrunedSpawns(const Board board, const double prob) const {
    auto possibleSpawns = bh->getPossibleSpawns(board);
    if (possibleSpawns.empty()) {
        return {};
    }
    vector<TilePosProb> ret;
    for (const auto &pos : possibleSpawns) {
        ret.emplace_back(1, pos, 0.9 / possibleSpawns.size());
        ret.emplace_back(2, pos, 0.1 / possibleSpawns.size());
    }
    return ret;
}

vector<ExpectiMax::BoardMoveProb> ExpectiMax::getPrunedMoves(const double prob,
                                                             const std::vector<BoardAndMove> &possibleMoves) const
{
    vector<BoardMoveProb> ret;
    if (possibleMoves.empty()) {
        return ret;
    }
    ret.reserve(possibleMoves.size());

    for (const auto &move : possibleMoves) {
        ret.emplace_back(move.board, move.move, 1./possibleMoves.size());
    }

    return ret;
}
