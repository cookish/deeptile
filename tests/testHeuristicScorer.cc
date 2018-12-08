//
// Created by Martin Cook on 2018/11/29.
//

#include "catch.hpp"
#include "HeuristicScorer.hh"

#include <memory>
#include <iostream>
using std::cout;
using std::endl;

#include <cmath>
using std::pow;


TEST_CASE(" Score calculated", "[heuristicScorer]" ) {
    HeuristicScorer h(std::make_shared<BoardHandler>(std::make_unique<RowHandler>()));
    h.setBase(2);
    SECTION("Sum zig zag") {
        REQUIRE(h.sumZigZag(0x0000111232112235) == 32+8+4+4+2+2);
        REQUIRE(h.sumZigZag(0x0101010101010101) == 2);
        REQUIRE(h.sumZigZag(0x1111111111111111) == 16*2);
        REQUIRE(h.sumZigZag(0x9876543210000000) == 512+256+128+64+32+16+8+4+2);
        REQUIRE(h.sumZigZag(0x9876000012345678) == 512+256+128+64);
    }
    SECTION("Sum longest monotonic from corner") {
//        REQUIRE(h.sumAlongLongestMonotonicCornerPath(0x5210003400000000) == 32+16+8+4+2);
        REQUIRE(h.sumAlongLongestMonotonicCornerPath(0x5433421110000000) == 32+16+8+4+2+2+2);

    }
    SECTION("Get score") {
        REQUIRE(h.getScoreSpawned(0x3321) == 22);
        REQUIRE(h.getScoreSpawned(0x2100000001122000) == Approx(10.58));

    }
}

