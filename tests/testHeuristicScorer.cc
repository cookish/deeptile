//
// Created by Martin Cook on 2018/11/29.
//

#include "catch.hpp"
#include "HeuristicScorer.hh"

#include <iostream>
using std::cout;
using std::endl;

HeuristicScorer h;

TEST_CASE( " Score calculated", "[heuristicScorer]" ) {
//    REQUIRE(h.getScore(0x3321) == 22);
    REQUIRE(h.getScore(0x2100000001122000) == 8);
}
