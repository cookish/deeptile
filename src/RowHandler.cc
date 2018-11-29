//
// Created by Martin Cook on 2018/11/29.
//

#include "RowHandler.hh"

#include <iostream>
using std::cout;
using std::endl;

/*
 Row notation:
 3 2 1 0
 */

RowHandler::RowHandler() {
    initCache();
}

void RowHandler::initCache() {
    for (Row r = 0; r <= 0xFFFF; r++) {
        moveCache[r] = moveLeftInner(r);
        if (r == 65535) break; // break needed, otherwise r overflows to 0 again, and loop goes on forever
    }
}

Board RowHandler::moveLeft(const Board row) const {
    return moveCache[row];
}

Row RowHandler::moveLeftInner(Row row) const {

    // first, move into open spaces. Start at right hand side
    if ((row & 0xF0) == 0) {  // 1 is open
        row |= ((row & 0xF) << 4); // 1 gets value from 0
        row &= ~0xF; // clear 0
    }
    if ((row & 0xF00) == 0) { // 2 is open
        row |= ((row & 0xF0) << 4); // 2 gets value from 1
        row &= ~0xF0; // clear 1
        row |= ((row & 0xF) << 4); // 1 gets value from 0
        row &= ~0xF; // clear 0
    }
    if ((row & 0xF000) == 0) { // 3 is open
        row |= ((row & 0xF00) << 4); // 3 gets value from 2
        row &= ~0xF00; // clear 2
        row |= ((row & 0xF0) << 4); // 2 gets value from 1
        row &= ~0xF0; // clear 1
        row |= ((row & 0xF) << 4); // 1 gets value from 0
        row &= ~0xF; // clear 0
    }

    // now, combine tiles. Start at left hand side
    if ((row & 0xF000) == 0) return row; // don't combine zeros.
    if ((row & 0xF000) == (row & 0xF00) << 4) {  // 3 == 2
        row += 0x1000;  // add one to 3
        row &= ~0xF00;   // clear 2
        row |= ((row & 0xF0) << 4); // 2 gets value from 1
        row &= ~0xF0; // clear 1
        row |= ((row & 0xF) << 4); // 1 gets value from 0
        row &= ~0xF; // clear 0
    }

    if ((row & 0xF00) == 0) return row; // don't combine zeros.
    if ((row & 0xF00) == (row & 0xF0) << 4) {  // 2 == 1
        row += 0x100;  // add one to 2
        row &= ~0xF0; // clear 1
        row |= ((row & 0xF) << 4); // 1 gets value from 0
        row &= ~0xF; // clear 0
    }

    if ((row & 0xF0) == 0) return row; // don't combine zeros.
    if ((row & 0xF0) == (row & 0xF) << 4) {  // 1 == 0
        row += 0x10;  // add one to 1
        row &= ~0xF; // clear 0
    }
    return row;

}
