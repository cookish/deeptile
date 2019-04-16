//
// Created by Martin Cook on 2019-01-30.
//

#ifndef DEEPTILE_OUTPUT_HH
#define DEEPTILE_OUTPUT_HH

#include "ExpectiMax.hh"

#include <string>
using std::string;

class Output
{
public:
    static string formatMoveInfo(Board board, const vector<MoveBoardScoreEffort> &moves);
    static string formatBoard(Board board);
    static string formatBoardHex(Board board, int indent = 0);
    static string getMoveName(int move);

private:
    static string getTileChar(int tile);
};


#endif //DEEPTILE_OUTPUT_HH
