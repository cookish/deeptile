//
// Created by Martin Cook on 2018/11/28.
//

#ifndef MLTWO_BOARDHANDLER_HH
#define MLTWO_BOARDHANDLER_HH

#include "RowHandler.hh"

#include <cstdint>

using Board = uint64_t;

class BoardHandler
{
public:
    explicit BoardHandler(std::unique_ptr<RowHandler> rowHandler) : rowHandler(std::move(rowHandler)) {;}
    Board getPrincipalBoard(Board board) const;
    Board rotateLeft(Board board) const;
    Board flip(Board board) const;
    Board moveLeft(Board board) const;
    void printHex(Board board) const;

private:
    std::unique_ptr<RowHandler> rowHandler;
};


#endif //MLTWO_BOARDHANDLER_HH
