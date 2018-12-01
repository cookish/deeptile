//
// Created by Martin Cook on 2018/11/30.
//

#include <ScoreCache.hh>

#include "ScoreCache.hh"

double ScoreCache::getScore(Board pBoard, int gen) const {
//    return -1;
    auto f = cache.find(pBoard);
    if (f == cache.end()) {
        return -2.;
    }
    if (f->second.gen > gen) {
        return -1.;
    }
//    auto score = f->second.score;
    return f->second.score;
}

void ScoreCache::insertScoreSafe(Board pBoard, int gen, double score) {
    // we first need to check whether a better cache value already exists
    auto s = getScore(pBoard, gen);
    if (s < 0) {
        cache[pBoard] = {score, gen};
    }
}

void ScoreCache::insertScore(Board board, int gen, double score) {
    cache[board] = ScoreGen{score, gen};
}
