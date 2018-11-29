//
// Created by Martin Cook on 2018/11/28.
//

#include "BoardHandler.hh"

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <BoardHandler.hh>

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
Board BoardHandler::rotateLeft(const Board board) const {
    Board newBoard = (board & 0xF) << 48  // 0 -> 12
        | (board & 0xF0) << 28  // 1 -> 8
        | (board & 0xF00) << 8  // 2 -> 4
        | (board & 0xF000) >> 12    // 3 -> 0
        | (board & 0xF0000) << 36  // 4 -> 13
        | (board & 0xF00000) << 16  // 5 -> 9
        | (board & 0xF000000) >> 4  // 6 -> 5
        | (board & 0xF0000000) >> 24  // 7 -> 1
        | (board & 0xF00000000) << 24  // 8 -> 14
        | (board & 0xF000000000) << 4  // 9 -> 10
        | (board & 0xF0000000000) >> 16  // 10 -> 6
        | (board & 0xF00000000000) >> 36  // 11 -> 2
        | (board & 0xF000000000000) << 12  // 12 -> 15
        | (board & 0xF0000000000000) >> 8  // 13 -> 11
        | (board & 0xF00000000000000) >> 28  // 14 -> 7
        | (board & 0xF000000000000000) >> 48  // 15 -> 3
        ;
    return newBoard;
}

// flip around axis through 0, 5, 10, 15
Board BoardHandler::flip(const Board board) const {
    Board newBoard = (board & 0xF)  // 0
        | (board & 0xF0) << 12  // 1 -> 4
        | (board & 0xF00) << 24  // 2 -> 8
        | (board & 0xF000)  << 36  // 3 -> 12
        | (board & 0xF0000) >> 12   // 4 -> 1
        | (board & 0xF00000)        // 5
        | (board & 0xF000000) << 12  // 6 -> 9
        | (board & 0xF0000000) << 24  // 7 -> 13
        | (board & 0xF00000000) >> 24  // 8 -> 2
        | (board & 0xF000000000) >> 12  // 9 -> 6
        | (board & 0xF0000000000)        // 10
        | (board & 0xF00000000000) << 12  // 11 -> 14
        | (board & 0xF000000000000) >> 36  // 12 -> 3
        | (board & 0xF0000000000000) >> 24  // 13 -> 7
        | (board & 0xF00000000000000) >> 12  // 14 -> 11
        | (board & 0xF000000000000000)         // 15
        ;
    return newBoard;
}

Board BoardHandler::moveLeft(const Board board) const {
    Board newBoard = rowHandler->moveLeft(board & 0xFFFF)
        | rowHandler->moveLeft((board & 0xFFFF0000) >> 16) << 16
        | rowHandler->moveLeft((board & 0xFFFF00000000) >> 32) << 32
        | rowHandler->moveLeft((board & 0xFFFF000000000000) >> 48) << 48
        ;
    return newBoard;
}

void BoardHandler::printHex(const Board board) const {
    cout << std::setfill('0') << std::setw(4) << std::hex << (board >> 12*4) << endl;
    cout << std::setfill('0') << std::setw(4) << std::hex << ((board >> 8*4) & 0xFFFF) << endl;
    cout << std::setfill('0') << std::setw(4) << std::hex << ((board >> 4*4) & 0xFFFF) << endl;
    cout << std::setfill('0') << std::setw(4) << std::hex << (board & 0xFFFF) << endl;
    cout << "\n";
}
