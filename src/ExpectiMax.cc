//
// Created by Martin Cook on 2018/11/30.
//

#include "ExpectiMax.hh"

#include <iostream>
using std::cout;
using std::endl;

double ExpectiMax::getBestMoveRecurse(Board board, int &move, int gens, int &numEvals, int indent) {
    move = -1;

    if (gens == 0) {
        numEvals = 1;
        ++stats->leafEvals;
        return scorer->getScore(board);
    }

//    for (int i = 0; i < indent; i++) cout << " ";
//    cout << "getBestMoveRecurse: evaluating board:" << endl;
//    bh->printHex(board, indent);


    double score;
    double bestScore = scoreForDeath;
    auto possibleMoves = bh->getPossibleMoves(board);
    for (const auto &currentMove : possibleMoves) {
        int tmpEvals = 0;
        score = getAverageSpawnRecurse(currentMove.board, gens, tmpEvals, indent + 2);
        numEvals += tmpEvals;
        if (score > bestScore) {
            move = currentMove.move;
            bestScore = score;
        }
    }
//    for (int i = 0; i < indent; i++) cout << " ";
//    cout << "getBestMoveRecurse: returning score:" << bestScore << endl;
    return bestScore;
}


double ExpectiMax::getAverageSpawnRecurse(Board board, int gens, int &numEvals, int indent) {
//    for (int i = 0; i < indent; i++) cout << " ";
//    cout << "getAverageSpawnRecurse: evaluating board:" << endl;
//    bh->printHex(board, indent);
    Board spawnedBoard;
    auto possibleTiles = bh->getPossibleSpawns(board);

    int cachedEvals = 0;
    int cacheHits = 0;
    int cacheMisses = 0;
    int numNodes = 0;
    double score = 0;
    int tempMove = 0;
    auto newGens = gens - 1;
    for (Board tile = 1; tile <= 2; ++tile) {
        double prob = 0.9 - (tile - 1) * 0.8; // 0.9 if tile==0, 0.1 if tile == 1
        for (const auto &i : possibleTiles) {
            spawnedBoard = board | (tile << (4 * i));
            auto pBoard = bh->getPrincipalBoard(spawnedBoard);
//            auto newGens = gens - static_cast<int>(tile);
            auto cacheVal = cache->get(pBoard, newGens);
            ++numNodes;
            if (cacheVal.score >= 0) {  // found a cached result
                score += cacheVal.score * prob;
                cachedEvals += cacheVal.numEvals;
                ++cacheHits;
            } else {
                int tmpEvals = 0;
                auto calcScore = getBestMoveRecurse(spawnedBoard, tempMove, newGens, tmpEvals, indent + 2);
                numEvals += tmpEvals;
                score += calcScore * prob;
                cache->insertScoreSafe(pBoard, newGens, calcScore, tmpEvals);
                ++cacheMisses;
            }
        }
    }
    numEvals += cachedEvals;
    stats->cachedEvalsPerGen[newGens] += cachedEvals;
    stats->totalEvalsPerGen[newGens] += numEvals;
    stats->cachedEvals += cachedEvals;
    stats->cacheHitsPerGen[newGens] += cacheHits;
    stats->cacheMissesPerGen[newGens] += cacheMisses;
    stats->nodesPerGen[newGens] += numNodes;

    score = score / possibleTiles.size();
//    for (int i = 0; i < indent; i++) cout << " ";
//    cout << "getAverageSpawnRecurse: returning score:" << score << endl;
    return score;
}

