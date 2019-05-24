//
// Created by Martin Cook on 2018/11/28.
//

#ifndef MLTWO_BOARDHANDLER_HH
#define MLTWO_BOARDHANDLER_HH

#include "RowHandler.hh"

#include <memory>
#include <cstdint>
#include <vector>
using std::vector;
#include <string>

using Board = uint64_t;
using TileList = uint64_t;

struct BoardAndMove {
    Board board;
    int move;
};

class BoardHandler
{
public:
    explicit BoardHandler(std::unique_ptr<RowHandler> rowHandler) : rowHandler(std::move(rowHandler)) {;}
    Board getPrincipalBoard(Board board) const;
    Board rotateLeft(Board board) const;
    Board flip(Board board) const;
    Board moveLeft(Board board) const;
    int moveAndScore(Board &board, int move) const;
    inline int getSpawnFromList(const TileList tile, int index) const {
        return static_cast<int>((tile >> (index*4 + 4)) & 0xF);
    }
    TileList getPossibleSpawns(Board board) const;
    vector<BoardAndMove> getPossibleMoves(Board board) const;
    bool isDead(Board board) const;
    int getBoardTotal(Board board) const;
    int getHighestTile(Board board) const;

    static inline int getTileValue(const Board board, const int pos) {
        return static_cast<int>((board >> (pos * 4)) & 0xF);
    }
    static inline int getExpFromValue(const int value) {
        return (value == 0) ? 0 : (1 << value);
    }
    static std::string toJson(const Board board);

private:
    std::unique_ptr<RowHandler> rowHandler;
};


#endif //MLTWO_BOARDHANDLER_HH
