//
// Created by Martin Cook on 2018-12-04.
//

#include "JsonStats.hh"
using nlohmann::json;

#include <RunStats.hh>


nlohmann::json JsonStats::create(const RunStats &rs) {
    json j(rs.d);
    j["cacheHitsPerGen"] = json(rs.cacheHitsPerGen);
    j["cacheMissesPerGen"] = json(rs.cacheMissesPerGen);
    j["totalEvalsPerGen"] = json(rs.totalEvalsPerGen);
    j["cachedEvalsPerGen"] = json(rs.cachedEvalsPerGen);
    j["nodesPerGen"] = json(rs.nodesPerGen);
    return j;
}
