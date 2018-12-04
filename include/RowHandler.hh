//
// Created by Martin Cook on 2018/11/29.
//

#ifndef MLTWO_MOVEHANDLER_HH
#define MLTWO_MOVEHANDLER_HH

#include <cstdint>
#include <array>

using Row = uint16_t;
using Board = uint64_t;

class RowHandler
{
public:
    RowHandler();
    // these functions take boards instead of rows, to prevent many casting conversions.
    Board moveLeft(Board row) const;
    Board moveLeft(Board row, int & score) const;
    int getTotal(Board row) const;

private:
    void initCache();
    Row moveLeftInner(Row row, int & score) const;
    std::array<Row, 65536> moveCache;
    std::array<int, 65536> scoreCache;
    std::array<int, 65536> totalCache;
};


#endif //MLTWO_MOVEHANDLER_HH
