//
// Created by Martin Cook on 2018/11/29.
//

#include "HeuristicScorer.hh"
#include "BoardHandler.hh"
#include "Settings.hh"

#include <vector>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <cmath>
#include <Output.hh>


using std::cout;
using std::endl;

using std::vector;

HeuristicScorer::HeuristicScorer(std::shared_ptr<BoardHandler> bh, const Settings* settings)
    : bh(std::move(bh)), settings(settings)
{
    neighbourList[0] =  0x17;
    neighbourList[1] =  0x260;
    neighbourList[2] =  0x135;
    neighbourList[3] =  0x24;
    neighbourList[4] =  0x35b;
    neighbourList[5] =  0x246a;
    neighbourList[6] =  0x1579;
    neighbourList[7] =  0x680;
    neighbourList[8] =  0x79f;
    neighbourList[9] =  0x68ae;
    neighbourList[10] = 0x59bd;
    neighbourList[11] = 0x4ac;
    neighbourList[12] = 0xbd;
    neighbourList[13] = 0xace;
    neighbourList[14] = 0x9df;
    neighbourList[15] = 0x8e;

    belowList[0] = -1;
    belowList[1] = -1;
    belowList[2] = -1;
    belowList[3] = -1;
    belowList[4] = 3;
    belowList[5] = 2;
    belowList[6] = 1;
    belowList[7] = 0;
    belowList[8] = 7;
    belowList[9] = 6;
    belowList[10] = 5;
    belowList[11] = 4;
    belowList[12] = 11;
    belowList[13] = 10;
    belowList[14] = 9;
    belowList[15] = 8;
    setBase(2.2);
}

void HeuristicScorer::setBase(double b) {
    base = b;
    initTileScores();
}

void HeuristicScorer::initTileScores() {
    tileScore[0] = 0;
    for (int i=1; i<16; i++) {
        tileScore[i] = std::pow(base, i);
    }
}

double HeuristicScorer::getScoreSpawned(Board board) {
    int option = settings->algorithm_option;

    if (option == 1) { // repeated test along zig zag, plus a fraction along longest monotonic path from a corner
        double fracInZigZag = 0.9;
        auto score =
            fracInZigZag * repeatMonotonic(board) + (1 - fracInZigZag) * sumAlongLongestMonotonicCornerPath(board);
        return score;
    }
    if (option == 2) {
        double fracInZigZag = 0.9;
        auto score =
            fracInZigZag * sumZigZag(board) + (1 - fracInZigZag) * sumAlongLongestMonotonicCornerPath(board);
        return score;
    }
    // zigzag plus tail
    if (option == 3) {
        auto score = sumZigZagPlusTail(board);
        return score;
    }
    if (option == 4) {
        double fracInZigZag = 0.9;
        auto score =
            fracInZigZag * sumZigZagPlusTail(board) + (1 - fracInZigZag) * sumAlongLongestMonotonicCornerPath(board);
        return score;
    }
    if (option == 5) {
        return board;
    }
    return 0.;
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

double HeuristicScorer::sumZigZagPlusTail(Board board) const {
    const double tailWeight = 0.5;
    auto pBoard = bh->getPrincipalBoard(board);
//    bh->printHex(pBoard);
    double score = 0;
    auto prevValue = 0xF;
    int pos;
    int val;
    for (pos=15; pos>=0; --pos) {
        val = bh->getTileValue(pBoard, pos);
        if (val > prevValue || val == 0) {
            break;
        }
        score += tileScore[val];
        prevValue = val;
    }
    ++pos;
    val = bh->getTileValue(pBoard, pos);
//    cout << Output::formatBoard(board) << endl;
    score -= tileScore[val];
    score += sumAlongLongestMonotonicPathRecurse(board, pos, 0xFFFF, 0) * tailWeight;
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
//    bh->printBoard(pBoard);
    auto score = sumAlongLongestMonotonicPathRecurse(pBoard, 15, 0xFFFF, 0);
    return score;
}

double HeuristicScorer::sumAlongLongestMonotonicPathRecurse(const Board board,
                                                            const int currentPos,
                                                            const int parentVal,
                                                            int indent) const
{
    bool print = false;
    if (print) for (int i =0; i < indent; i++) printf( " ");
    if (print) printf( "Investigating pos: %d\n ",currentPos );
    auto currentVal = BoardHandler::getTileValue(board, currentPos);
    if (currentVal == 0 || currentVal > parentVal) {
        if (print) for (int i =0; i < indent; i++) printf( " ");
        if (print)  printf( "Pos %d is a dead end\n", currentPos);
        return 0;
    }
    double max = 0;
    double val;
    auto n = neighbourList[currentPos];
//    for (const auto &pos : getNeighbours(currentPos)) {

    // set the current tile to zero, so that the algorithm does not come back here
    Board newBoard = board & ~(0xFull << (currentPos*4));

    auto x = n & 0xF;
    val = sumAlongLongestMonotonicPathRecurse(newBoard, n & 0xF, currentVal, indent+3);
    if (val > max) { max = val; }

    x = (n & 0xF0) >> 4;
    val = sumAlongLongestMonotonicPathRecurse(newBoard, (n & 0xF0) >> 4, currentVal, indent+3);
    if (val > max) { max = val; }

    if ((n & 0xF00) != 0) {  // three neighbours
        x = (n & 0xF00) >> 8;
        val = sumAlongLongestMonotonicPathRecurse(newBoard, (n & 0xF00) >> 8, currentVal, indent+3);
        if (val > max) { max = val; }

        if ((n & 0xF000) != 0) {  // four neighbours
            x = (n & 0xF000) >> 12;
            val = sumAlongLongestMonotonicPathRecurse(newBoard, (n & 0xF000) >> 12, currentVal, indent+3);
            if (val > max) { max = val; }
        }
    }
    if (print) for (int i =0; i < indent; i++) printf( " ");
    if (print) printf( "Pos %d returning %f\n", currentPos, max + tileScore[currentVal]);
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

double HeuristicScorer::repeatMonotonic(Board board) const {
    auto pBoard = bh->getPrincipalBoard(board);
    int current = 15;
    int previous = 0xF;
    int previousPos = -1;
    double score = 0.;
//    bh->printBoard(pBoard);
    double factor = 1.;
    while (current >= 0) {

        auto val = bh->getTileValue(pBoard, current);
        while (val == 0 && current > 0) {
            val = bh->getTileValue(pBoard, --current);
            previous = 0;
        }
        if (current == 0) break;

        if (val <= previous) {
            score += tileScore[val] * factor;
//            score += bh->getExpFromValue(val) * factor;
//            if (val < previous - 1 && previousPos > 0 && belowList[previousPos] > 0) {
//                score += factor * 0.4 * sumAlongLongestMonotonicPathRecurse(pBoard, belowList[previousPos], val, 0);
//            }
        } else {
//            factor *= (1 - (val - previous) / 15.);
//            if (factor <= 0) factor = 0.1;
            factor *= 0.5;
            score += tileScore[val] * factor;
//            break;
//            for (int i = current-1; i >= 0; --i) {
//                score -= bh->getExpFromValue(bh->getTileValue(pBoard, i));
//            }
        }

        previous = val;
        previousPos = current;
        --current;
    }
//    cout << "Returning score: " << score << endl;
    return static_cast<double>(score);
}
