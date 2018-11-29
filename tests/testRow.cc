//
// Created by Martin Cook on 2018/11/29.
//

#include "catch.hpp"
#include "RowHandler.hh"

#include <iostream>
using std::cout;
using std::endl;

TEST_CASE( " Rows are moved left", "[moveHandler]" ) {
    RowHandler rh;
    auto row = Row{0x1102};
    REQUIRE(rh.moveLeft(row) == 0x2200);
}
