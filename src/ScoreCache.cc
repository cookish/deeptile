//
// Created by Martin Cook on 2018/11/30.
//

#include "ScoreCache.hh"

CacheVal ScoreCache::get(Board pBoard, int gen) const {
//    return -1;
    auto f = cache.find(pBoard);
    if (f == cache.end()) {
        return {-2., -2, -2};
    }
    if (f->second.gen > gen) {
        return {-1., -1, -1};
    }
//    auto score = f->second.score;
    return f->second;
}

void ScoreCache::insertScoreSafe(Board pBoard, int gen, double score, int numEvals) {
    // we first need to check whether a better cache value already exists
    auto s = get(pBoard, gen);
    if (s.score < 0) {
        insertScore(pBoard, gen, score, numEvals);
    }
}

void ScoreCache::insertScore(Board pBoard, int gen, double score, int numEvals) {
    cache[pBoard] = CacheVal{score, gen, numEvals};
}
