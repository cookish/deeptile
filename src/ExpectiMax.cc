//
// Created by Martin Cook on 2018/11/30.
//

#include "ExpectiMax.hh"

#include <iostream>
using std::cout;
using std::endl;

double ExpectiMax::getBestMoveRecurse(Board board, int &move, int gen, int indent) {
    move = -1;

    if (gen >= genLimit) {
        return scorer->getScore(board);
    }

//    for (int i = 0; i < indent; i++) cout << " ";
//    cout << "getBestMoveRecurse: evaluating board:" << endl;
//    bh->printHex(board, indent);


    double score;
    double bestScore = scoreForDeath;
    auto possibleMoves = bh->getPossibleMoves(board);
    for (const auto &currentMove : possibleMoves) {
        score = getAverageSpawnRecurse(currentMove.board, gen, indent+2);
        if (score > bestScore) {
            move = currentMove.move;
            bestScore = score;
        }
    }
//    for (int i = 0; i < indent; i++) cout << " ";
//    cout << "getBestMoveRecurse: returning score:" << bestScore << endl;
    return bestScore;
}


double ExpectiMax::getAverageSpawnRecurse(Board board, int gen, int indent) {
//    for (int i = 0; i < indent; i++) cout << " ";
//    cout << "getAverageSpawnRecurse: evaluating board:" << endl;
//    bh->printHex(board, indent);
    Board spawnedBoard;
    auto possibleTiles = bh->getPossibleSpawns(board);

    double score = 0;
    int tempMove = 0;
    auto newGen = gen + 1;
    for (Board tile = 1; tile <= 2; ++tile) {
        double prob = 0.9 - (tile - 1) * 0.8; // 0.9 if tile==0, 0.1 if tile == 1
        for (const auto &i : possibleTiles) {
            spawnedBoard = board | (tile << (4 * i));
            auto pBoard = bh->getPrincipalBoard(spawnedBoard);
//            auto newGen = gen + static_cast<int>(tile);
            auto cacheScore = cache->getScore(pBoard, newGen);
            if (cacheScore >= 0) {
                score += cacheScore * prob;
            } else {
                auto calcScore = getBestMoveRecurse(spawnedBoard, tempMove, newGen, indent + 2);
                score += calcScore * prob;
                cache->insertScoreSafe(pBoard, newGen, calcScore);
            }
        }
    }
    score = score / possibleTiles.size();
//    for (int i = 0; i < indent; i++) cout << " ";
//    cout << "getAverageSpawnRecurse: returning score:" << score << endl;
    return score;
}

