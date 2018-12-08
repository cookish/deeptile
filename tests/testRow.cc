//
// Created by Martin Cook on 2018/11/29.
//

#include "catch.hpp"
#include "RowHandler.hh"

#include <iostream>
using std::cout;
using std::endl;

TEST_CASE( " Rows are moved ", "[moveHandler]" ) {
    RowHandler rh;
    auto row = Row{0x1102};
    REQUIRE(rh.moveLeft(row) == 0x2200);
    REQUIRE(rh.moveRight(row) == 0x0022);
    row = Row{0x2111};
    REQUIRE(rh.moveLeft(row) == 0x2210);
    REQUIRE(rh.moveRight(row) == 0x0212);
}

TEST_CASE( " Max tiles are combined", "[moveHandler]" ) {
    RowHandler rh;
    REQUIRE(rh.moveLeft(0xFFFF) == 0xFF00);
    REQUIRE(rh.moveLeft(0xF00F) == 0xF000);
}

TEST_CASE( " Score is calculated ", "[moveHandler]" ) {
    RowHandler rh;
    int score;
    rh.moveRight(0x3355, score);
    REQUIRE(score == 16 + 64);
    rh.moveLeft(0x0011, score);
    REQUIRE(score == 4);
    rh.moveLeft(0x00AA, score);
    REQUIRE(score == 2048);
    rh.moveLeft(0x00EE, score);
    REQUIRE(score == 32768);
    rh.moveLeft(0xF0F0, score);
    REQUIRE(score == 65536);
}