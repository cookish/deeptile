//
// Created by Martin Cook on 2018/11/29.
//

#include "catch.hpp"
#include "HeuristicScorer.hh"

#include <memory>
#include <iostream>
using std::cout;
using std::endl;

HeuristicScorer h(std::make_shared<BoardHandler>(std::make_unique<RowHandler>()));

TEST_CASE( " Score calculated", "[heuristicScorer]" ) {
//    REQUIRE(h.get(0x3321) == 22);
    REQUIRE(h.getScoreSpawned(0x2100000001122000) == Approx(10.58));
}
