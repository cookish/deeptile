//
// Created by Martin Cook on 2019-04-18.
//

#include "DataLogger.hh"
#include "BoardHandler.hh"

#include <fstream>
#include <iostream>
#include <Output.hh>
using std::cout;
using std::endl;

// Note: does not convert to principal board
void DataLogger::logDeadBoard(const Board board) {
    deadBoards.emplace_back(board);
}

// Note: does not convert to principal board
void DataLogger::logCalculatedBoard(Board board, double score, int gensAboveLeaf) {
    calculatedBoards.emplace_back(board);
    calculatedScores.emplace_back(score + gensAboveLeaf);
}

// Note: does not convert to principal board
void DataLogger::logMove(Board board) {
    moves.emplace_back(board);
}

void DataLogger::writeToFiles(const string &deadFile,
                              const string &calculatedBoardFile,
                              const string &calculatedScoreFile,
                              const string &moveFile) const {
    std::ofstream deadOut(deadFile.c_str(), std::ios::out | std::ios::binary);
    deadOut.write(reinterpret_cast<const char*>(deadBoards.data()),
                  deadBoards.size() * sizeof(Board));

    std::ofstream calculatedBoardOut(calculatedBoardFile.c_str(), std::ios::out | std::ios::binary);
    calculatedBoardOut.write(reinterpret_cast<const char*>(calculatedBoards.data()),
                             calculatedBoards.size() * sizeof(Board));

    std::ofstream calculatedScoreOut(calculatedScoreFile.c_str(), std::ios::out | std::ios::binary);
    calculatedScoreOut.write(reinterpret_cast<const char*>(calculatedScores.data()),
                             calculatedScores.size() * sizeof(Board));

    std::ofstream movesOut(moveFile.c_str(), std::ios::out | std::ios::binary);
    movesOut.write(reinterpret_cast<const char*>(moves.data()),
                   moves.size() * sizeof(Board));

}
