//
// Created by Martin Cook on 2018/11/29.
//

#include "catch.hpp"
#include "BoardHandler.hh"

#include <iostream>
using std::cout;
using std::endl;

BoardHandler b(std::make_unique<RowHandler>());

TEST_CASE( "Boards are rotated", "[boardHandler]" ) {
    auto board = Board{0xfedcba9876543210};
    REQUIRE(b.rotateLeft(board) == 0xcb4325ade961078f);
}

TEST_CASE( "Boards are flipped", "[boardHandler]" ) {
    auto board = Board{0xfedcba9876543210};
    REQUIRE(b.flip(board) == 0x34bcda52169ef870);
}

TEST_CASE( "Principal value is calculated", "[boardHandler]" ) {
    auto board = Board{0xfedcba9876543210};
    REQUIRE(b.getPrincipalBoard(board) == board);
    board = Board{0x12};
    REQUIRE(b.getPrincipalBoard(board) == 0x2100000000000000);
}

TEST_CASE( "Boards are moved left", "[boardHandler]" ) {
    auto board = Board{0x1102400003038888};
    REQUIRE(b.moveLeft(board) == 0x2200000440000099);
}