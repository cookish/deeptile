//
// Created by Martin Cook on 2018/11/28.
//

#include "BoardHandler.hh"

#include <algorithm>
#include <iostream>
#include <BoardHandler.hh>
#include <sstream>
#include <array>
using std::array;
#include <string>
using std::string;

using std::cout;
using std::endl;


/*
 Board layout:
 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
 _____________________
 | 15 | 14 | 13 | 12 |
 | 11 | 10 |  9 |  8 |
 |  7 |  6 |  5 |  4 |
 |  3 |  2 |  1 |  0 |
 ---------------------
 */


Board BoardHandler::getPrincipalBoard(Board board) const {
    Board max = board;
    for (int i = 0; i < 3; i++) {
        board = rotateLeft(board);
        max = std::max<Board>(board, max);
    }
    board = flip(board);
    max = std::max<Board>(board, max);
    for (int i = 0; i < 3; i++) {
        board = rotateLeft(board);
        max = std::max<Board>(board, max);
    }
    return max;
}

// rotate 90 degrees counterclockwise
// TODO: add ull for unsigned long long
Board BoardHandler::rotateLeft(const Board board) const {
    Board newBoard = (board & 0xF) << 12    // 0 -> 3
        | (board & 0xF0) << 12  // 1 -> 4
        | (board & 0xF00) << 36  // 2 -> 11
        | (board & 0xF000) << 36  // 3 -> 12
        | (board & 0xF0000) << 36  // 4 -> 13
        | (board & 0xF00000) << 20  // 5 -> 10
        | (board & 0xF000000) >> 4  // 6 -> 5
        | (board & 0xF0000000) >> 20  // 7 -> 2
        | (board & 0xF00000000) >> 28  // 8 -> 1
        | (board & 0xF000000000) >> 12  // 9 -> 6
        | (board & 0xF0000000000) >> 4  // 10 -> 9
        | (board & 0xF00000000000) << 12  // 11 -> 14
        | (board & 0xF000000000000) << 12  // 12 -> 15
        | (board & 0xF0000000000000) >> 20  // 13 -> 8
        | (board & 0xF00000000000000) >> 28  // 14 -> 7
        | (board & 0xF000000000000000) >> 60  // 15 -> 0
    ;
    return newBoard;
}

// flip around axis through 0, 6, 10, 12
Board BoardHandler::flip(const Board board) const {
    Board newBoard = (board & 0xF)  // 0
        | (board & 0xF0) << 24  // 1 -> 7
        | (board & 0xF00) << 24  // 2 -> 8
        | (board & 0xF000)  << 48  // 3 -> 15
        | (board & 0xF0000) << 40   // 4 -> 14
        | (board & 0xF00000) << 16      // 5 -> 9
        | (board & 0xF000000)         // 6
        | (board & 0xF0000000) >> 24  // 7 -> 1
        | (board & 0xF00000000) >> 24  // 8 -> 2
        | (board & 0xF000000000) >> 16  // 9 -> 5
        | (board & 0xF0000000000)        // 10
        | (board & 0xF00000000000) << 8  // 11 -> 13
        | (board & 0xF000000000000)       // 12
        | (board & 0xF0000000000000) >> 8  // 13 -> 11
        | (board & 0xF00000000000000) >> 40  // 14 -> 4
        | (board & 0xF000000000000000) >> 48       // 15 -> 3
    ;
    return newBoard;
}

Board BoardHandler::moveLeft(const Board board) const {
    Board newBoard = rowHandler->moveRight(board & 0xFFFF)
        | rowHandler->moveLeft((board & 0xFFFF0000) >> 16) << 16
        | rowHandler->moveRight((board & 0xFFFF00000000) >> 32) << 32
        | rowHandler->moveLeft((board & 0xFFFF000000000000) >> 48) << 48
    ;
    return newBoard;
}

int BoardHandler::moveAndScore(Board &board, int move) const {
    for (int i = 0; i < move; ++i) {
        board = rotateLeft(board);
    }
    int score = 0;
    int tempScore = 0;
    Board newBoard = 0;
    newBoard |= rowHandler->moveRight(board & 0xFFFF, tempScore);
    score += tempScore;
    newBoard |= rowHandler->moveLeft((board & 0xFFFF0000) >> 16, tempScore) << 16;
    score += tempScore;
    newBoard |= rowHandler->moveRight((board & 0xFFFF00000000) >> 32, tempScore) << 32;
    score += tempScore;
    newBoard |= rowHandler->moveLeft((board & 0xFFFF000000000000) >> 48, tempScore) << 48;
    score += tempScore;
    board = newBoard;
    if (move > 0) {
        for (int i = 0; i < (4 - move); ++i) {
            board = rotateLeft(board);
        }
    }
    return score;
}

vector<BoardAndMove> BoardHandler::getPossibleMoves(Board board) const {
    vector<BoardAndMove> v;
    Board movedBoard;
    for (int m = 0; m < 4; ++m) {
        movedBoard = moveLeft(board);
        if (movedBoard != board) {
            v.push_back({movedBoard, m});
        }
        if (m != 4) board = rotateLeft(board);  // don't need to rotate last time
    }
    return v;
}

bool BoardHandler::isDead(Board board) const {
    Board movedBoard = moveLeft(board);
    for (int i = 0; i < 3; ++i) {
        if (movedBoard != board) return false;
        board = rotateLeft(board);
        movedBoard = moveLeft(board);
    }
    return (movedBoard == board);
}

TileList BoardHandler::getPossibleSpawns(Board board) const {
    Board ret = 0;
    int numSpawns=0;
    for (Board pos = 0; pos < 16; ++pos) {
        if ((board & (0xFull << (pos*4))) == 0) {
            ret += (pos << (++numSpawns * 4));
        }
    }
    ret += numSpawns;
    return ret;
}

int BoardHandler::getBoardTotal(const Board board) const {
    auto score = rowHandler->getTotal(board & 0xFFFF)
        + rowHandler->getTotal((board >> 16) & 0xFFFF)
        + rowHandler->getTotal((board >> 32) & 0xFFFF)
        + rowHandler->getTotal((board >> 48) & 0xFFFF);
    return score;
}

int BoardHandler::getHighestTile(const Board board) const {
    int tile;
    int max = 0;
    for (int i = 0; i < 16; i++) {
        tile = getTileValue(board, i);
        if (tile > max) {
            max = tile;
        }
    }
    return max;
}

// hand code this, probably faster than using json library, don't need to add to project dependencies
string BoardHandler::toJson(Board board) {
    auto positions = array<int, 16>{15, 14, 13, 12, 8, 9, 10, 11, 6, 5, 4, 3, 0, 1, 2, 3};
    Board tileMask = 0xFull;
    auto boardArr = array<array<int, 4>, 4>{};
    for (int outer = 0; outer < 4; ++outer) {
        for (int inner = 0; inner < 4; ++inner) {
            auto index = outer * 4 + inner;
            boardArr[outer][inner] = (board >> (positions[index] * 4)) & tileMask;
        }
    }
    std::stringstream ss;
    string outerSeparator{};
    for (const auto &outer : boardArr) {
        ss << outerSeparator  << "[";
        outerSeparator = ",";
        std::string innerSeparator{};
        for (const auto &inner : outer) {
            ss << innerSeparator << "[" << inner << "]";
            innerSeparator = ",";
        }
        ss << "]";
    }
    auto str = ss.str();
    return str;
}

