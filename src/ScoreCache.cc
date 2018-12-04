//
// Created by Martin Cook on 2018/11/30.
//

#include "ScoreCache.hh"

#include <iostream>
using std::cout;
using std::endl;

CacheVal ScoreCache::get(Board pBoard, int gen) const {
//    return -1;
    auto f = cache.find(pBoard);
    if (f == cache.end()) { // no cache value
        return {-2., -2, -2};
    }
    if (f->second.gen < gen) { // cached value was evaluated at fewer generations
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

void ScoreCache::prune(Board board, BoardHandler *bh) {
//    cout << "size before: " << cache.size() << endl;
    cache.clear();
    // The code below prunes nicely, but gains not worth the cost. Try again when generations are deeper.
//    auto thresh = bh->getBoardTotal(board);
//    decltype(cache) newCache;
//    for (auto &&i : cache) {
//        if (bh->getBoardTotal(i.first) >= thresh) {
//            newCache.emplace(i);
//        }
//    }
//    cache.swap(newCache);
//    cout << "size after: " << cache.size() << endl;
}
