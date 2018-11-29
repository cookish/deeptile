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
    Board moveLeft(Board row) const;

private:
    void initCache();
    Row moveLeftInner(Row row) const;
    std::array<Row, 65536> moveCache;
};


#endif //MLTWO_MOVEHANDLER_HH
