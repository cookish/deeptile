//
// Created by Martin Cook on 2019-01-30.
//


#include "Output.hh"

#include <sstream>
#include <iomanip>

string Output::formatMoveInfo(const Board board, const vector <MoveBoardScoreEffort> &moves) {
    std::stringstream ss;
    ss << std::left;
    for (const auto &pos : {15, 14, 13, 12}) {
        auto tile = BoardHandler::getTileValue(board, pos);
        ss << std::setw(5);
        ss << getTileChar(tile);
    }
    ss << "  Hex: " << formatBoardHex(board) << "\n";

    for (const auto &pos : {8, 9, 10, 11}) {
        auto tile = BoardHandler::getTileValue(board, pos);
        ss << std::setw(5);
        ss << getTileChar(tile);
    }
    ss << "  Scores:";
    bool hasEffort = false;
    for (const auto &move : moves) {
        ss << " " << getMoveName(move.move) << " (" << move.score << ")";
        if (move.effort > 0) hasEffort = true;
    }
    ss << "\n";

    for (const auto &pos : {7, 6, 5, 4}) {
        auto tile = BoardHandler::getTileValue(board, pos);
        ss << std::setw(5);
        ss << getTileChar(tile);
    }
    if (hasEffort) {
        ss << "  Effort:";
        for (const auto &move : moves) {
            ss << " " << getMoveName(move.move) << " (" << move.effort << ")";
        }
    }
    ss << "\n";

    for (const auto &pos : {0, 1, 2, 3}) {
        auto tile = BoardHandler::getTileValue(board, pos);
        ss << std::setw(5);
        ss << getTileChar(tile);
    }
    if (!moves.empty()) {
        ss << "  Best move: " << getMoveName(moves[0].move) << " " << formatBoardHex(moves[0].board);
    }
    ss << "\n";

    return ss.str();
}

string Output::formatBoardHex(const Board board, int indent) {
    std::stringstream ss;
    ss << std::hex;
    for (int i = 15; i >= 0; --i) {
        auto val = BoardHandler::getTileValue(board, i);
        if (val > 0) ss << val; else ss << ".";
        if (i % 4 == 0 && i > 0) ss << "|";
    }
    return ss.str();
}

string Output::formatBoard(const Board board) {
    std::stringstream ss;
    ss << std::left;
    int i = 0;
    for (const auto &pos : {15, 14, 13, 12, 8, 9, 10, 11, 7, 6, 5, 4, 0, 1, 2, 3}) {
        auto tile = BoardHandler::getTileValue(board, pos);
        ss << std::setw(5);
        ss << getTileChar(tile);
        if (i % 4 == 3) ss << "\n";
        ++i;
    }
    ss << "\n";
    return ss.str();
}

string Output::getTileChar(int tile) {
    return (tile == 0) ? "." : std::to_string(1 << tile);

}

string Output::getMoveName(int move) {
    switch(move) {
        case 0: return "left";
        case 1: return "up";
        case 2: return "right";
        case 3: return "down";
        default:
            return "error";
    }
}
