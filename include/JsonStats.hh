//
// Created by Martin Cook on 2018-12-04.
//

#ifndef MLTWO_JSONSTATS_HH
#define MLTWO_JSONSTATS_HH

#include <nlohmann/json.hpp>
class RunStats;

class JsonStats
{
public:
    static nlohmann::json create(const RunStats &rs);
};


#endif //MLTWO_JSONSTATS_HH
