//
// Created by Martin Cook on 2019-04-18.
//

#ifndef DEEPTILE_DATALOGGER_HH
#define DEEPTILE_DATALOGGER_HH

#include "BoardHandler.hh"

#include <vector>
using std::vector;

#include <string>
using std::string;

class DataLogger
{
public:
    void logDeadBoard(Board board);
    void logCalculatedBoard(Board board, double score);
    void logMove(Board board);
    void writeToFiles(const string &deadFile,
                      const string &calculatedBoardFile,
                      const string &calculatedScoreFile,
                      const string &moveFile) const;

private:
    vector<Board> deadBoards;
    vector<Board> calculatedBoards;
    vector<Board> calculatedScores;
    vector<Board> moves;
};


#endif //DEEPTILE_DATALOGGER_HH
