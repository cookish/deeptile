//
// Created by Martin Cook on 2018/11/29.
//

#include "HeuristicScorer.hh"
#include "BoardHandler.hh"

#include <vector>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <cmath>


using std::cout;
using std::endl;

using std::vector;

HeuristicScorer::HeuristicScorer(std::shared_ptr<BoardHandler> bh)
    : bh(std::move(bh))
{
    neighbourList[0] =  0x17;
    neighbourList[1] =  0x260;
    neighbourList[2] =  0x135;
    neighbourList[3] =  0x24;
    neighbourList[4] =  0x35b;
    neighbourList[5] =  0x246a;
    neighbourList[6] =  0x1579;
    neighbourList[7] =  0x068;
    neighbourList[8] =  0x79f;
    neighbourList[9] =  0x68ae;
    neighbourList[10] = 0x59bd;
    neighbourList[11] = 0x4ac;
    neighbourList[12] = 0xbd;
    neighbourList[13] = 0xace;
    neighbourList[14] = 0x9df;
    neighbourList[15] = 0x8e;

    tileScore[0] = 0;
    for (int i=1; i<16; i++) {
        tileScore[i] = std::pow(base, i);
    }
}

double HeuristicScorer::getScoreSpawned(Board board) {
//    cout << "==> Scorer: Evaluating board" << endl;
//    bh.printHex(board);

//    auto score = sumAlongLongestMonotonicCornerPath(board);
//    auto score = sumZigZag(board);
    auto score = 1./3.*sumAlongLongestMonotonicCornerPath(board) + 2./3.*sumZigZag(board);
//    cout << "==> Evaluated: score " << score << endl;
    return score;
}

double HeuristicScorer::sumZigZag(Board board) const {
    auto pBoard = bh->getPrincipalBoard(board);
//    bh->printHex(pBoard);
    double score = 0;
    auto prevValue = 0xF;
    for (int pos=0; pos<16; ++pos) {
        auto val = bh->getTileValue(pBoard, (15 - pos));
        if (val > prevValue || val == 0) {
            break;
        }
        score += tileScore[val];
        prevValue = val;
    }
    return score;
}

double HeuristicScorer::sumAlongLongestMonotonicPath(Board board) const {
    auto max = findMaxValue(board);
    auto max_score = 0.;
    for (int i = 0; i < 16; i++) {
        if (BoardHandler::getTileValue(board, i) == max) {
            max_score = std::max(max_score, sumAlongLongestMonotonicPathRecurse(board, i, max, 0));
        }
    }
    return max_score;
}

double HeuristicScorer::sumAlongLongestMonotonicCornerPath(Board board) const {
    auto pBoard = bh->getPrincipalBoard(board);
    auto score = sumAlongLongestMonotonicPathRecurse(pBoard, 15, 0xFFFF, 0);
    return score;
}

double HeuristicScorer::sumAlongLongestMonotonicPathRecurse(const Board board,
                                                            const int currentPos,
                                                            const int parentVal,
                                                            int indent) const
{
    bool print = false;
    if (print) for (int i =0; i < indent; i++) cout << " ";
    if (print) cout << "Investigating pos: " << currentPos << endl;
    auto currentVal = BoardHandler::getTileValue(board, currentPos);
    if (currentVal == 0 || currentVal > parentVal) {
        if (print) for (int i =0; i < indent; i++) cout << " ";
        if (print)  cout << "Pos " << currentPos << " is a dead end" << endl;
        return 0;
    }
    double max = 0;
    double val;
    auto n = neighbourList[currentPos];
//    for (const auto &pos : getNeighbours(currentPos)) {

    // set the current tile to zero, so that the algorithm does not come back here
    Board newBoard = board & ~(0xFull << (currentPos*4));

    val = sumAlongLongestMonotonicPathRecurse(newBoard, n & 0xF, currentVal, indent+3);
    if (val > max) { max = val; }

    val = sumAlongLongestMonotonicPathRecurse(newBoard, (n & 0xF0) >> 4, currentVal, indent+3);
    if (val > max) { max = val; }

    if ((n & 0xF00) != 0) {  // three neighbours
        val = sumAlongLongestMonotonicPathRecurse(newBoard, (n & 0xF00) >> 8, currentVal, indent+3);
        if (val > max) { max = val; }

        if ((n & 0xF000) != 0) {  // four neighbours
            val = sumAlongLongestMonotonicPathRecurse(newBoard, (n & 0xF000) >> 12, currentVal, indent+3);
            if (val > max) { max = val; }
        }
    }
    if (print) for (int i =0; i < indent; i++) cout << " ";
    if (print) cout << "Pos " << currentPos << " returning " << max + BoardHandler::getExpFromValue(currentVal) << endl;
    return max + tileScore[currentVal];
}

int HeuristicScorer::findMaxValue(Board board) const {
    auto max = (board & 0xF);
    do {
        board = board >> 4;
        max = std::max(max, board & 0xF);
    } while (board > 0);
    return static_cast<int>(max);
}

double HeuristicScorer::getScoreMovedFast(const Board board) {
    return static_cast<double>(bh->getPrincipalBoard(board));
}
