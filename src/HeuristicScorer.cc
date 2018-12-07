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
    neighbourList[0] =  0x14;
    neighbourList[1] =  0x250;
    neighbourList[2] =  0x136;
    neighbourList[3] =  0x27;
    neighbourList[4] =  0x05;
    neighbourList[5] =  0x1469;
    neighbourList[6] =  0x257a;
    neighbourList[7] =  0x36b;
    neighbourList[8] =  0x49c;
    neighbourList[9] =  0x58ad;
    neighbourList[10] = 0x69be;
    neighbourList[11] = 0x7af;
    neighbourList[12] = 0x8d;
    neighbourList[13] = 0x9ce;
    neighbourList[14] = 0xadf;
    neighbourList[15] = 0xbe;
}

double HeuristicScorer::getScore(Board board) {
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
    auto prevValue = 0xFFFF;
    for (const auto & pos : {15, 14, 13, 12, 8, 9, 10, 11, 7, 6, 5, 4, 0, 1, 2, 3}) {
        auto val = bh->getTileValue(pBoard, pos);
        if (val > prevValue || val == 0) {
            break;
        }
//        score += (1 << val);
//        score += val;
        score += std::pow(base, val);
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
    return max + std::pow(base, currentVal);
}

int HeuristicScorer::findMaxValue(Board board) const {
    auto max = (board & 0xF);
    do {
        board = board >> 4;
        max = std::max(max, board & 0xF);
    } while (board > 0);
    return static_cast<int>(max);
}