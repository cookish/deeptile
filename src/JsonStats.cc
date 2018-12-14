//
// Created by Martin Cook on 2018-12-04.
//

#include "JsonStats.hh"
using nlohmann::json;

#include <RunStats.hh>


nlohmann::json JsonStats::create(const RunStats &rs) {
    json j(rs.values);
    for (const auto &valPerGen : rs.valuesPerGen) {
        j[valPerGen.first] = json(valPerGen.second);
    }
    return j;
}
