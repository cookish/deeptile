//
// Created by Martin Cook on 2018/11/29.
//

#include "HeuristicScorer.hh"
#include "BoardHandler.hh"

#include <vector>
#include <algorithm>
#include <iostream>

using std::cout;
using std::endl;

using std::vector;

double HeuristicScorer::getScore(Board board) {
//    auto score = sumAlongLongestMonotonicPath(board);
//    auto bh = BoardHandler(std::make_unique<RowHandler>());
//    cout << "==> Scorer: Evaluating board" << endl;
//    bh.printHex(board);

//    auto score = sumAlongLongestMonotonicCornerPath(board);
//    auto score = sumZigZag(board);
    auto score = 1./3.*sumAlongLongestMonotonicCornerPath(board) + 2./3.*sumZigZag(board);
//    cout << "==> Evaluated: score " << score << endl;
    return score;
}

int HeuristicScorer::sumZigZag(Board board) const {
    auto pBoard = bh->getPrincipalBoard(board);
//    bh->printHex(pBoard);
    int score = 0;
    auto prevValue = 0xFFFFull;
    for (const auto & pos : {15, 14, 13, 12, 8, 9, 10, 11, 7, 6, 5, 4, 0, 1, 2, 3}) {
        auto val = (pBoard & (0xFull << (4 * pos))) >> (4 * pos);
        if (val > prevValue || val == 0) {
            break;
        }
        score += (1 << val);
        prevValue = val;
    }
    return score;
}

int HeuristicScorer::sumAlongLongestMonotonicPath(Board board) const {
    auto max = findMaxValue(board);
    auto max_score = 0;
    for (int i = 0; i < 16; i++) {
        if (BoardHandler::getTileValue(board, i) == max) {
            max_score = std::max(max_score, sumAlongLongestMonotonicPathRecurse(board, i, max, 0));
        }
    }
    return max_score;
}

int HeuristicScorer::sumAlongLongestMonotonicCornerPath(Board board) const {
    auto pBoard = bh->getPrincipalBoard(board);
    auto score = sumAlongLongestMonotonicPathRecurse(pBoard, 15, 0xFFFF, 0);
    return score;
}

int HeuristicScorer::sumAlongLongestMonotonicPathRecurse(const Board board,
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
    int max = 0;
    int val;
    for (const auto &pos : getNeighbours(currentPos)) {
        // set the current tile to zero, so that the algorithm does not come back here
        // ull means unsigned long long
        val = sumAlongLongestMonotonicPathRecurse(board & ~(0xFull << (currentPos * 4)), pos, currentVal, indent+3);
        if (val > max) {
            max = val;
        }

    }
    if (print) for (int i =0; i < indent; i++) cout << " ";
    if (print) cout << "Pos " << currentPos << " returning " << max + BoardHandler::getExpFromValue(currentVal) << endl;
    return max + BoardHandler::getExpFromValue(currentVal);
}

int HeuristicScorer::findMaxValue(Board board) const {
    auto max = (board & 0xF);
    do {
        board = board >> 4;
        max = std::max(max, board & 0xF);
    } while (board > 0);
    return static_cast<int>(max);
}

vector<int> HeuristicScorer::getNeighbours(int pos) const {
    switch (pos) {
        case 0: return vector<int>{1, 4};
        case 1: return vector<int>{2, 5, 0};
        case 2: return vector<int>{1, 3, 6};
        case 3: return vector<int>{2, 7};
        case 4: return vector<int>{0, 5};
        case 5: return vector<int>{1, 4, 6, 9};
        case 6: return vector<int>{2, 5, 7, 10};
        case 7: return vector<int>{3, 6, 11};
        case 8: return vector<int>{4, 9, 12};
        case 9: return vector<int>{5, 8, 10, 13};
        case 10: return vector<int>{6, 9, 11, 14};
        case 11: return vector<int>{7, 10, 15};
        case 12: return vector<int>{8, 13};
        case 13: return vector<int>{9, 12, 14};
        case 14: return vector<int>{10, 13, 15};
        case 15: return vector<int>{11, 14};
        default: return {};
    }
}
