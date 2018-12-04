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
 d c b a
 */

RowHandler::RowHandler() {
    initCache();
}

void RowHandler::initCache() {
    for (Row r = 0; r <= 0xFFFF; r++) {
        int score;
        moveCache[r] = moveLeftInner(r, score);
        scoreCache[r] = score;

        auto total = 0;
        for (int i=0; i < 4; i++) {
            auto tile = (r & (0xF << (4*i))) >> (4*i);
            total += (tile == 0) ? 0 : (1 << tile);
        }
        totalCache[r] = total;
        if (r == 65535) break; // break needed, otherwise r overflows to 0 again, and loop goes on forever
    }
}

Board RowHandler::moveLeft(const Board row) const {
    return moveCache[static_cast<Row>(row)];
}


// gets both the moved row and the 2048 game score from any combined tiles
Board RowHandler::moveLeft(Board row, int &score) const {
    score = scoreCache[static_cast<Row>(row)];
    return moveCache[static_cast<Row>(row)];
}

int RowHandler::getTotal(Board row) const {
    return totalCache[row];
}

// moves a row to the left.
// two max value tiles (F in hex) combined to form another F
// also calculates the 2048 game score from any combinations
Row RowHandler::moveLeftInner(Row row, int &score) const {
    score = 0;
    int a = row & 0xF;
    int b = (row & 0xF0) >> 4;
    int c = (row & 0xF00) >> 8;
    int d = (row & 0xF000) >> 12;

    // first, move into open spaces. Start at right hand side
    if (b == 0) {  // 1 is open
        b = a;
        a = 0;
    }
    if (c == 0) { // 2 is open
        c = b;
        b = a;
        a = 0;
    }
    if ((row & 0xF000) == 0) { // 3 is open
        d = c;
        c = b;
        b = a;
        a = 0;
    }

    // now, combine tiles. Start at left hand side
    if (d == c && d != 0) {  // 3 == 2
        score += (2 << d);
        if (d != 0xF) d += 1;  // add one to 3
        c = b; // 2 gets value from 1
        b = a;
        a = 0;
    }

    if (c == b && c != 0) {  // 2 == 1
        score += (2 << c);
        if (c != 0xF) c += 1;  // add one to 2
        b = a;
        a = 0;
    }

    if (b == a && b != 0) {  // 1 == 0
        score += (2 << b);
        if (b != 0xF) b += 1;  // add one to 1
        a = 0;
    }
    return static_cast<Row>((d << 12) + (c << 8) + (b << 4) + a);
}

